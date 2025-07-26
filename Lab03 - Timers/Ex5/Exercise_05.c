#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t overflow_count = 0;

void timer0_init() {
    TCNT0 = 0x00;               //initial counter value = 0

    TCCR0A = 0x00;              //set the timer0 to normal mode

    TCCR0B = 0x05;              // prescaler value = 1024
    TIMSK0 = (1 << TOIE0);      
    sei();                     
}

ISR(TIMER0_OVF_vect) {
    overflow_count++;
    if (overflow_count >= 6) {  
        PORTB ^= (1 << PB5);    /* Toggle PB5 */
        overflow_count = 0;
    }
}

int main(void) {
    DDRB |= (1 << PB5);        
    timer0_init();

    while (1);                  
}
