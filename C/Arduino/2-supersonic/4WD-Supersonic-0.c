#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// Motor pins
#define LW_F PD2
#define LW_I PD4
#define LW_EN PD3
#define RW_F PD6
#define RW_I PD7
#define RW_EN PD5

// Ultrasonic pins
#define TRIG_PIN PB0  // D8
#define ECHO_PIN PB1  // D9

int defaultSpeed = 200;

void setupPins() {
    // Motor pins as output
    DDRD |= (1 << LW_F) | (1 << LW_I) | (1 << RW_F) | (1 << RW_I);
    DDRD |= (1 << LW_EN) | (1 << RW_EN); // Enable pins for PWM

    // Ultrasonic pins
    DDRB |= (1 << TRIG_PIN);  // TRIG output
    DDRB &= ~(1 << ECHO_PIN); // ECHO input
}

void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint8_t lw_speed, uint8_t rw_speed) {
    // Left motor
    if (lw_f) PORTD |= (1 << LW_F); else PORTD &= ~(1 << LW_F);
    if (lw_i) PORTD |= (1 << LW_I); else PORTD &= ~(1 << LW_I);
    // Right motor
    if (rw_f) PORTD |= (1 << RW_F); else PORTD &= ~(1 << RW_F);
    if (rw_i) PORTD |= (1 << RW_I); else PORTD &= ~(1 << RW_I);

    // Speed (PWM): dùng analogWrite() vì PORTx không điều khiển được PWM
    analogWrite(3, lw_speed); // pin D3
    analogWrite(5, rw_speed); // pin D5
}

void forward() {
    setMotor(1, 0, 1, 0, defaultSpeed, defaultSpeed);
}

void stop() {
    setMotor(0, 0, 0, 0, 0, 0);
}

uint16_t readDistanceCM() {
    // Gửi xung
    PORTB &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTB |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG_PIN);

    // Đo độ rộng xung
    while (!(PINB & (1 << ECHO_PIN))); // đợi lên cao
    uint32_t count = 0;
    while (PINB & (1 << ECHO_PIN)) {
        _delay_us(1);
        count++;
    }

    // Tính khoảng cách (cm)
    return (uint16_t)(count * 0.034 / 2);
}

int main(void) {
    setupPins();

    while (1) {
        uint16_t distance = readDistanceCM();
        if (distance < 20) {
            stop();
        } else {
            forward();
        }
        _delay_ms(100);
    }
}
