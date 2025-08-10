#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t direction = 1;
volatile uint8_t position = 0;

void timer1_init() {
    TCNT1 = 49911;               /* Load for 1s delay (65536 - 15625) */
    TCCR1A = 0x00;               
    TCCR1B = 0x05;               /* Prescaler = 1024 */
    TIMSK1 = (1 << TOIE1);       
    sei();                       /* Enable global interrupts */
}

ISR(TIMER1_OVF_vect) {
    TCNT1 = 49911;               
    PORTB ^= (1 << PB5);         
}

void knight_rider() {
    PORTB = (PORTB & 0xF0) | (1 << position);  /* PB0–PB3 LED pattern */
    _delay_ms(100);
    position += direction;
    if (position == 3 || position == 0) direction = -direction;
}

int main(void) {
    DDRB = 0x3F;                 
    timer1_init();

    while (1) {
        knight_rider();         
    }
}
