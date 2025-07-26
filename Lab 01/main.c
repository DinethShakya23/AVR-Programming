#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 200

int main(void) {
    /*
    unsigned char z;
    DDRB = 0x0F; 

    while (1) {
        for (z = 0; z < 15; z++) {
            PORTB = z;
            _delay_ms(BLINK_DELAY_MS);
        }
    }

    return 0;
    */

    DDRB = 0x0F;  // Set PB0–PB3 as output

    uint8_t pattern[] = {0x01, 0x02, 0x04, 0x08, 0x04, 0x02};
    uint8_t i;

    while (1) {
        for (i = 0; i < sizeof(pattern); i++) {
            PORTB = pattern[i];
            _delay_ms(BLINK_DELAY_MS);
        }
    }

    return 0;
}
