#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRD = DDRD & ~(1 << PD2);      // Set PD2 (INT0) as input
    PORTD = PORTD | (1 << PD2);     // Enable pull-up on PD2

    DDRB = DDRB | (1 << PB0);       // Set PB0 as output
    PORTB = PORTB & ~(1 << PB0);    // Start with LED off

    EICRA = EICRA | (1 << ISC01) | (1 << ISC00);  // Rising edge on INT0
    EIMSK = EIMSK | (1 << INT0);   // Enable external interrupt INT0

    sei();                         // Enable global interrupts

    while (1)
    {
        // Main loop does nothing; all work is in ISR
    }

    return 0;
}

ISR(INT0_vect)
{
    PORTB = PORTB ^ (1 << PB0);    // Toggle PB0
}
