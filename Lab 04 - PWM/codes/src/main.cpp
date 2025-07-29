//Exercise 01

// #include <avr/io.h>
// #include <util/delay.h>

// int main(void) {

//     DDRD |= (1 << DDD6);  // PD6 is OC0A - output pin for PWM

//     OCR0A = 128;  // 50% duty cycle - 50% of 255

//     // Step 3: Configure Fast PWM mode
//     // WGM01,0 = 11 for Fast PWM (TCCR0A)
//     // COM0A1,0 = 10 for non-inverting mode (TCCR0A)
//     TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
//     TCCR0A &= ~(1 << COM0A0);  // Ensure COM0A0 is 0

//     // Step 4: Set prescaler to 64 → CS01 and CS00 = 1 (CS02,CS01,CS00 - 0 1 1)
//     // WGM02 = 0 is already default
//     TCCR0B |= 0x03;

//     // PWM is now running on PD6 with ~976.56Hz, 50% duty cycle

//     while (1) {
//         // main loop does nothing
//     }

//     return 0;
// }



//Exercise 02

// #include <avr/io.h>
// #include <util/delay.h>

// int main(void) {
//     // Step 1: Set OC0A (PD6) as output
//     DDRD |= (1 << DDD6);  // PD6 is OC0A

//     // Step 2: Set duty cycle to 50%
//     OCR0A = 0;  // 50% of 255

//     // Step 3: Configure Fast PWM mode

//     TCCR0A |= 0b11000011;  // Set COM0A1, WGM01, and WGM00 and low COM0A0

//     // Step 4: Set prescaler to 64 → CS01 and CS00 = 1
//     // WGM02 = 0 is already default
//     TCCR0B |= 0x03;

//     // PWM is now running on PD6 with ~976.56Hz, 50% duty cycle

//     while (1) {
//         for (OCR0A = 0; OCR0A < 255; OCR0A++){
//           _delay_ms(8);  // Delay to observe the change in duty cycle
//         }
//         for(OCR0A = 255; OCR0A > 0; OCR0A--){
//           _delay_ms(8);  // Delay to observe the change in duty cycle
//         }
//     }
//     return 0;
// }




// //Exercise 03

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Step 1: Set OC0A (PD6) as output
    DDRD |= (1 << DDD6);  // PD6 is OC0A

    // Step 2: Set duty cycle to 50%
    OCR0A = 128;  // 50% of 255

    // Step 3: Configure Fast PWM mode
    // WGM01:0 = 11 for Fast PWM (TCCR0A)
    // COM0A1:0 = 10 for non-inverting mode (TCCR0A)
    TCCR0A |= 0b11000011;

    // Step 4: Set prescaler to 64 → CS01 and CS00 = 1
    TCCR0A |= 0x03; // Set prescaler to 64 (CS02, CS01, CS00 = 0 1 1)
    
    while (1) {
      for(int i=0; i<4; i++){
        // set preScalar
        TCCR0B &= ~(1<<CS00 | 1<<CS01 | 1<<CS02); // Clear all prescaler bits
        switch (i){
          case 0:
            TCCR0B |= (1<<CS01);// Set prescaler to 8
            _delay_ms(1000); // Wait for 1 second
            break;
          case 1:
            TCCR0B |= (1<<CS01) | (1<<CS00); // Set prescaler to 64
            _delay_ms(1000); // Wait for 1 second
            break;
          case 2:
            TCCR0B |= (1<<CS02); // Set prescaler to 256
            _delay_ms(1000); // Wait for 1 second
            break;
          case 3:
            TCCR0B = (1<<CS02) | (1<<CS00); // Set prescaler to 1024
            _delay_ms(1000); // Wait for 1 second
            break;
        }
        // PORTD ^= (1 << PD6); // Toggle PD6 output
        _delay_ms(1000); // Wait for 1 second
      }    
    }

    return 0;
}
