#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    unsigned char count = 0;
    unsigned char pressed = 0;

    DDRD &= ~(1 << PD7);     // PD7 as input
    PORTD |= (1 << PD7);     // Enable internal pull-up on PD7

    DDRB |= 0x3F;            // Set PB0–PB5 as outputs
    PORTB = 0x00;            // Clear all LEDs

    while (1)
    {
        if ((PIND & (1 << PD7)) == 0)    // If button is pressed (active low)
        {
            if (pressed == 0)
            {
                _delay_ms(20);          // Debounce delay
                if ((PIND & (1 << PD7)) == 0)
                {
                    count = (count + 1) & 0x3F; // Increment and wrap at 63
                    PORTB = (PORTB & ~0x3F) | (count & 0x3F); // Update LEDs
                    pressed = 1;
                }
            }
        }
        else
        {
            pressed = 0;                // Reset pressed flag when button released
        }
    }

    return 0;
}
