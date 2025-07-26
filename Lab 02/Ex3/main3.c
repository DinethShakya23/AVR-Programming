#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char count = 0;

int main(void)
{
    DDRD = DDRD & ~(1 << PD2);      // Set PD2 (INT0) as input
    PORTD = PORTD | (1 << PD2);     // Enable pull-up on PD2

    DDRB = DDRB | 0x3F;             // Set PB0–PB5 as outputs
    PORTB = 0x00;                   // Clear all LEDs

    EICRA = EICRA | (1 << ISC01) | (1 << ISC00);  // Rising edge on INT0
    EIMSK = EIMSK | (1 << INT0);    // Enable INT0

    sei();                          // Global interrupt enable

    while (1)
    {
        // Nothing here; all handled in ISR
    }

    return 0;
}

ISR(INT0_vect)
{
    _delay_ms(50);                  // Debounce delay

    if (PIND & (1 << PD2))          // Still high? Confirmed release
    {
        count = count + 1;
        PORTB = (PORTB & ~0x3F) | (count & 0x3F);  // Update LEDs
    }
}
