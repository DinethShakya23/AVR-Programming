#include <avr/io.h>

void delay_timer0() {
    TCNT0 = 131;                
    TCCR0A = 0x00;              /* Normal mode */
    TCCR0B = 0x04;             
    
    while ((TIFR0 & 0x01) == 0);  

    TCCR0A = 0x00;
    TCCR0B = 0x00;              /* Stop the timer */
    TIFR0 = 0x01;              
}

int main(void) {
    DDRB |= (1 << PB5);         /* Configure PB5 as output */
    
    while (1) {
        PORTB ^= (1 << PB5);    /* Toggle PB5 */
        delay_timer0();         /* 2ms delay */
    }
}
