#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#define PICO_LED_PIN            25
#define PICO_PWM_PIN            2
#define PICO_DIR_PIN1           0
#define PICO_DIR_PIN2           1
#define PICO_SERVO_PIN          28

#define PWM_DUTY_CYCLE          0.65 /* 65% */
#define PWM_DUTY_CYCLE_STOP     0

#define UART_ID                 uart0
#define BAUD_RATE               9600
#define DATA_BITS               8
#define STOP_BITS               1
#define PARITY                  UART_PARITY_NONE
#define UART_TX_PIN             17
#define UART_RX_PIN             16

#define HIGH                    1
#define LOW                     0
#define SERVO_MAX_VALUE         20000
#define SERVO_LEFT              500
#define SERVO_RIGHT             2500
#define SERVO_STRAIGHT          1500

#define VALUE_MAX_LOOP          5
#define VALUE_DELAY_US_LED      50000

int angleServo[5] = {500, 1000, 1500, 2000, 2500};
bool turnRight = false;
bool turnLeft = false;
bool goStraight = false;
const uint count_top = 1000;

static void setup(void);
static void servoControl(int angle);
static void runMotor(void);
static void motorBackw(void);
static void motorFordw(void);
static void stopMotor(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */