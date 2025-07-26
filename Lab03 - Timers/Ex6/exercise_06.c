#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t ovf_t0 = 0;

void timer0_init() {
    TCNT0 = 0x00;
    TCCR0A = 0x00;
    TCCR0B = 0x05;              /* Prescaler 1024 */
    TIMSK0 = (1 << TOIE0);
}

void timer1_init() {
    TCNT1 = 57723;              /* For 500ms delay */
    TCCR1A = 0x00;
    TCCR1B = 0x05;              /* Prescaler 1024 */
    TIMSK1 = (1 << TOIE1);
}

ISR(TIMER0_OVF_vect) {
    ovf_t0++;
    if (ovf_t0 >= 3) {          /* ~50ms = 3 overflows */
        PORTB ^= (1 << PB4);
        ovf_t0 = 0;
    }
}

ISR(TIMER1_OVF_vect) {
    TCNT1 = 57723;
    PORTB ^= (1 << PB5);        /* Toggle every 500ms */
}

int main(void) {
    DDRB |= (1 << PB4) | (1 << PB5); /* PB4 & PB5 output */
    timer0_init();
    timer1_init();
    sei();                      

    while (1);              
}




