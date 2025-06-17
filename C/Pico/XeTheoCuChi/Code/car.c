#include "car.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/sync.h"

volatile Direction currentDirection = DIR_NONE;
volatile MotorState requestedMotorState = MOTOR_STOP;
static volatile MotorState currentMotorState = MOTOR_STOP;
static volatile absolute_time_t lastStateChange = 0;

static bool repeating_timer_callback(struct repeating_timer *t) {
    // Đọc trạng thái yêu cầu một cách an toàn (dù nó là volatile)
    MotorState localRequestedState = requestedMotorState;
    handleMotorState(localRequestedState);
    return true; // Tiếp tục timer
}

/* RX interrupt handler */
void on_uart_rx() {
	static uint8_t ch = 0x00;

    while (uart_is_readable(UART_ID)) {
        ch = uart_getc(UART_ID);
		switch(ch)
		{
			case 'A': currentDirection = DIR_STRAIGHT; break; //straight
			case 'B': currentDirection = DIR_LEFT; break; //right
			case 'C': currentDirection = DIR_RIGHT; break; //left
			case 'D': requestedMotorState = MOTOR_FORWARD; break;
            case 'E': requestedMotorState = MOTOR_STOP; break;
            case 'F': requestedMotorState = MOTOR_BACKWARD; break;
			default: break;
		}
    }
}

static void setup(void)
{
	//int actual;
	pwm_config cfg;
	int UART_IRQ;
	
	/* Setup for GPIO */
	gpio_init(PICO_LED_PIN);
	gpio_set_dir(PICO_LED_PIN, GPIO_OUT); //LED

	gpio_init(PICO_SERVO_PIN);
	gpio_set_dir(PICO_SERVO_PIN, GPIO_OUT); //SERVO

	/* Setup for Motor */
	gpio_init(PICO_DIR_PIN1);
	gpio_set_dir(PICO_DIR_PIN1, GPIO_OUT);
	gpio_init(PICO_DIR_PIN2);
	gpio_set_dir(PICO_DIR_PIN2, GPIO_OUT);

	/* Setup for PWM */
	gpio_set_function(PICO_PWM_PIN, GPIO_FUNC_PWM);
    cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, count_top);
    pwm_init(pwm_gpio_to_slice_num(PICO_PWM_PIN), &cfg, true);
	pwm_set_gpio_level(PICO_PWM_PIN, PWM_DUTY_CYCLE_STOP * (count_top + 1));

	/* Setup for UART */
	//uart_init(UART_ID, 2400);
	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
	//actual = uart_set_baudrate(UART_ID, BAUD_RATE);
	uart_set_hw_flow(UART_ID, false, false);
	uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
	uart_set_fifo_enabled(UART_ID, false);
	
	UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
	irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
	uart_set_irq_enables(UART_ID, true, false);

	
    /* Setup for Repeating Timer for Motor Control */
    static struct repeating_timer motor_timer; // Khai báo timer
    // Thêm repeating timer, gọi hàm callback mỗi 50ms
    // Xử lý lỗi nếu không thêm được timer
    if (!add_repeating_timer_ms(50, repeating_timer_callback, NULL, &motor_timer)) {
        // Xử lý lỗi ở đây, ví dụ: bật LED sáng liên tục hoặc một mẫu nháy khác
        while(1) {
            gpio_put(PICO_LED_PIN, HIGH);
            busy_wait_ms(100);
            gpio_put(PICO_LED_PIN, LOW);
            busy_wait_ms(100);
        }
    }
    lastStateChange = get_absolute_time(); // Khởi tạo thời điểm thay đổi trạng thái cuối cùng
}

static void servoControl(int angle)
{
	//limit the servo
	if (angle < SERVO_LEFT) angle = SERVO_LEFT;
    if (angle > SERVO_RIGHT) angle = SERVO_RIGHT;

	gpio_put(PICO_SERVO_PIN, HIGH);
	sleep_us(angle);
	gpio_put(PICO_SERVO_PIN, LOW);
	sleep_us(SERVO_MAX_VALUE - angle);
}

static void runMotor(void)
{
	pwm_set_gpio_level(PICO_PWM_PIN, PWM_DUTY_CYCLE * (count_top + 1));
}

static void motorBackw(void)
{
	gpio_put(PICO_DIR_PIN1, HIGH);
	gpio_put(PICO_DIR_PIN2, LOW);
}

static void motorFordw(void)
{
	gpio_put(PICO_DIR_PIN1, LOW);
	gpio_put(PICO_DIR_PIN2, HIGH);
}

static void stopMotor(void)
{
	gpio_put(PICO_DIR_PIN1, LOW);
	gpio_put(PICO_DIR_PIN2, LOW);
	pwm_set_gpio_level(PICO_PWM_PIN, PWM_DUTY_CYCLE_STOP * (count_top + 1));
}

static void handleMotorState(MotorState localRequestedMotorState) {
    absolute_time_t now = get_absolute_time();

    if (localRequestedMotorState != currentMotorState) {
        // Nếu động cơ đang chạy và có yêu cầu thay đổi (kể cả dừng)
        // thì trước tiên phải dừng động cơ.
        if (currentMotorState != MOTOR_STOP) {
            stopMotor();
            currentMotorState = MOTOR_STOP;
            lastStateChange = now;
            // Không thực hiện ngay lệnh mới, đợi chu kỳ timer tiếp theo
            // để đảm bảo độ trễ MOTOR_TRANSITION_DELAY_MS được tôn trọng sau khi dừng.
            return;
        }

        // Tại điểm này, currentMotorState chắc chắn là MOTOR_STOP.
        // Kiểm tra xem đã đủ thời gian trễ kể từ lần dừng cuối cùng chưa.
        if (absolute_time_diff_us(lastStateChange, now) / 1000 < MOTOR_TRANSITION_DELAY_MS) {
            return; // Vẫn đang trong thời gian trễ, chưa cho phép chạy lại.
        }

        // Thời gian trễ đã qua, áp dụng trạng thái yêu cầu mới (nếu không phải là STOP).
        // Nếu localRequestedMotorState là MOTOR_STOP, thì không cần làm gì thêm vì đã dừng.
        switch (localRequestedMotorState) {
            case MOTOR_FORWARD:
                motorFordw(); // Đặt chiều trước
                runMotor();   // Sau đó chạy PWM
                currentMotorState = MOTOR_FORWARD;
                lastStateChange = now;
                break;
            case MOTOR_BACKWARD:
                motorBackw(); // Đặt chiều trước
                runMotor();   // Sau đó chạy PWM
                currentMotorState = MOTOR_BACKWARD;
                lastStateChange = now;
                break;
            case MOTOR_STOP:
                // currentMotorState đã là MOTOR_STOP rồi, và delay đã qua.
                // Không cần gọi stopMotor() lần nữa trừ khi có lý do đặc biệt.
                // Tuy nhiên, gọi lại cũng không sao vì stopMotor() là idempotent.
                // lastStateChange được cập nhật nếu có một yêu cầu 'STOP' mới được xử lý.
                // Nếu không có yêu cầu mới (localRequested == currentMotorState == STOP),
                // thì không vào nhánh if đầu tiên.
				stopMotor();
				currentMotorState = MOTOR_STOP;
                lastStateChange = now; // Cập nhật thời điểm nếu lệnh là STOP rõ ràng
                break;
        }
    }
    // Nếu localRequestedMotorState == currentMotorState, không làm gì cả.
}

int main()
{
	int angle = SERVO_STRAIGHT;

	/* Setup all things */
	setup();

    while (true)
	{
        gpio_put(PICO_LED_PIN, HIGH);
        sleep_us(VALUE_DELAY_US_LED);
        gpio_put(PICO_LED_PIN, LOW);
        sleep_us(VALUE_DELAY_US_LED);

		servoControl(angle);

        // Xử lý hướng lái từ UART (đọc và reset biến volatile một cách an toàn)
        Direction localDir;
        uint32_t saved_irq = save_and_disable_interrupts(); // Tắt tất cả ngắt tạm thời
        localDir = currentDirection;
        currentDirection = DIR_NONE; // Reset lại sau khi đọc
        restore_interrupts(saved_irq); // Khôi phục lại ngắt

		switch (localDir)
		{
			case DIR_STRAIGHT:
				angle = SERVO_STRAIGHT;
				break;
			case DIR_RIGHT:
				angle = SERVO_RIGHT;
				break;
			case DIR_LEFT:
				angle = SERVO_LEFT;
				break;
			default:
				break;
		}

		// currentDirection = DIR_NONE;
		//handleMotorState(requestedMotorState);

		sleep_ms(20);
    }
}
