#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char binary_value = 0;

int main(void)
{
    // PD2 = INT0 → Button A (inputs 0)
    // PD3 = INT1 → Button B (inputs 1)
    DDRD &= ~((1 << PD2) | (1 << PD3));         // Set PD2, PD3 as input
    PORTD |= (1 << PD2) | (1 << PD3);           // Enable pull-ups

    DDRB |= 0x3F;                               // PB0–PB5 as output
    PORTB = 0x00;                               // Start with all LEDs OFF

    // Configure INT0 and INT1 for rising edge
    EICRA |= (1 << ISC01) | (1 << ISC00);       // INT0: rising edge
    EICRA |= (1 << ISC11) | (1 << ISC10);       // INT1: rising edge
    EIMSK |= (1 << INT0) | (1 << INT1);         // Enable INT0 and INT1

    sei(); // Enable global interrupts

    while (1)
    {
        PORTB = binary_value & 0x3F;            // Display current value
    }

    return 0;
}

// Button A → INT0 → input 0
ISR(INT0_vect)
{
    _delay_ms(50);                              // Debounce
    if (PIND & (1 << PD2))                      // Confirm it's a real press
    {
        binary_value = (binary_value << 1);     // Shift left, input 0
    }
}

// Button B → INT1 → input 1
ISR(INT1_vect)
{
    _delay_ms(50);                              // Debounce
    if (PIND & (1 << PD3))                      // Confirm it's a real press
    {
        binary_value = (binary_value << 1) | 1; // Shift left, input 1
    }
}
