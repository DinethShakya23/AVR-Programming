// Exercise 01

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void ADC_init() {
    // Set ADC1 (PC1) as input (by default all pins are input)
    // Set voltage reference to AVCC, left adjust result, input channel ADC1
    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0); // REFS0=1 (AVCC), ADLAR=1 (left), MUX0=1 (ADC1)
    // Enable ADC, set prescaler to 128 (for 16MHz/128 = 125kHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t ADC_read() {
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for conversion to finish
    while (ADCSRA & (1 << ADSC));
    // Left justified: ADCH has the 8 MSBs of the 10-bit result
    return ADCH;
}

int main(void) {
    DDRD = 0xFF; // Set Port D as output (for LEDs)
    ADC_init();

    while (1) {
        uint8_t adc_val = ADC_read();
        PORTD = adc_val; // Output to LEDs
        _delay_ms(100);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////

// Exercise 02

// #include <avr/io.h>
// #define F_CPU 16000000UL
// #include <util/delay.h>

// #define LDR_THRESHOLD 100 // Adjust this value experimentally

// void ADC_init() {
//     // Set voltage reference to external AREF, right adjust result, input channel ADC0
//     ADMUX = (0 << REFS1) | (0 << REFS0) | (0 << ADLAR); // AREF, right justified, ADC0
//     // Enable ADC, set prescaler to 128 (for 16MHz/128 = 125kHz)
//     ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
// }

// uint16_t ADC_read() {
//     // Start conversion
//     ADCSRA |= (1 << ADSC);
//     // Wait for conversion to finish
//     while (ADCSRA & (1 << ADSC));
//     // Combine ADCL and ADCH for 10-bit result (right justified)
//     return (ADCL | (ADCH << 8));
// }

// int main(void) {
//     DDRB |= (1 << PB0); // Set PB0 as output (for LED)
//     ADC_init();

//     while (1) {
//         uint16_t adc_val = ADC_read();
//         if (adc_val < LDR_THRESHOLD) {
//             // Dark: turn LED ON
//             PORTB |= (1 << PB0);
//         } else {
//             // Light: turn LED OFF
//             PORTB &= ~(1 << PB0);
//         }
//         _delay_ms(100);
//     }
// }
