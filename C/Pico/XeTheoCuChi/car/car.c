#include "car.h"

typedef enum {
    DIR_NONE,
    DIR_STRAIGHT,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

volatile Direction currentDirection = DIR_NONE;

/* RX interrupt handler */
void on_uart_rx() {
	static uint8_t ch = 0x00;

	// turnRight = false;
	// turnLeft = false;
	// goStraight = false;

    while (uart_is_readable(UART_ID)) {
        ch = uart_getc(UART_ID);
		switch(ch)
		{
			case 'A': currentDirection = DIR_STRAIGHT; break; //straight
			case 'B': currentDirection = DIR_LEFT; break; //right
			case 'C': currentDirection = DIR_RIGHT; break; //left
			case 'D': runMotor(); motorFordw(); break;
			case 'E': stopMotor(); break;
			case 'F': runMotor(); motorBackw(); break;
			default: break;
		}
    }
}

static void setup(void)
{
	int actual;
	int UART_IRQ;
	pwm_config cfg;
	
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
    cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, count_top);
    pwm_init(pwm_gpio_to_slice_num(PICO_PWM_PIN), &cfg, true);
	gpio_set_function(PICO_PWM_PIN, GPIO_FUNC_PWM);
	pwm_set_gpio_level(PICO_PWM_PIN, PWM_DUTY_CYCLE_STOP * (count_top + 1));

	/* Setup for UART */
	uart_init(UART_ID, 2400);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
	actual = uart_set_baudrate(UART_ID, BAUD_RATE);
	uart_set_hw_flow(UART_ID, false, false);
	uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
	uart_set_fifo_enabled(UART_ID, false);
	
	UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
	irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
	uart_set_irq_enables(UART_ID, true, false);
}

static void servoControl(int angle)
{
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

		switch (currentDirection)
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

		currentDirection = DIR_NONE;

		sleep_ms(20);
    }
}
