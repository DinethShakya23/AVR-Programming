#include <avr/io.h>
#include <util/delay.h>

/////////////////////////////////////////////////Exercise 01///////////////////////////////////////////////////////

void usart_init(void) {

    UBRR0H = (103 >> 8); 
    UBRR0L = 103;        

    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Set frame format: 8-bit data, no parity, 1 stop bit, asynchronous
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Transmit a single character
void usart_send(char data) {
    // Wait until Transmit Data Buffer is empty (UDRE0 = 1)
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Write the character to UDR0 for transmission
    UDR0 = data;
}

// Receive a single character
char usart_receive(void) {
    // Wait until a byte is received (RXC0 = 1)
    while (!(UCSR0A & (1 << RXC0)));
    
    // Return the received character from UDR0
    return UDR0;
}

///////////////////////////////////////////////Exercise 02///////////////////////////////////////////////////////

#include <string.h>

// Include the functions from Exercise 1
void usart_init(void);
void usart_send(char data);

// Function to send a string (null-terminated)
void usart_send_string(const char *str) {
    while (*str) {
        usart_send(*str); // Send each character
        str++;
    }
}

int main(void) {
    // Initialize USART
    usart_init();
    
    const char *group_data[] = {
        "Name: Mineth De Croos, E no.: E/20/054\r\n",
        "Name: Dineth Shakya, E no.: E/20/055\r\n",
        NULL
    };

    // Loop forever
    while (1) {
        // Send each group mate's data
        for (int i = 0; group_data[i] != NULL; i++) {
            usart_send_string(group_data[i]);
            _delay_ms(1000); // Optional delay to avoid flooding the terminal
        }
        
    }

    return 0;
}



//////////////////////////////////////////////////exercise 03///////////////////////////////////////////////////////

// // Include Exercise 1 functions
// void usart_init(void);
// void usart_send(char data);
// char usart_receive(void);

// // Function to apply Caesar cipher (key = 3) to a character
// char caesar_cipher(char ch) {
//     // Encrypt only alphabetic characters (A-Z, a-z)
//     if (ch >= 'A' && ch <= 'Z') {
//         // Shift by 3, wrap around if exceeding 'Z'
//         return 'A' + ((ch - 'A' + 3) % 26);
//     } else if (ch >= 'a' && ch <= 'z') {
//         // Shift by 3, wrap around if exceeding 'z'
//         return 'a' + ((ch - 'a' + 3) % 26);
//     }
//     // Non-alphabetic characters remain unchanged
//     return ch;
// }

// int main(void) {
//     // Initialize USART
//     usart_init();
    
//     char received_char;
//     char buffer[100]; // Buffer to store the sentence (adjust size as needed)
//     int index = 0;

//     // Loop forever
//     while (1) {
//         // Receive a sentence until carriage return ('\r')
//         do {
//             received_char = usart_receive();
//             if (received_char != '\r') {
//                 buffer[index++] = received_char;
//             }
//         } while (received_char != '\r' && index < 99); // Prevent buffer overflow
        
//         // Null-terminate the buffer
//         buffer[index] = '\0';
        
//         // Encrypt and send the sentence
//         for (int i = 0; buffer[i] != '\0'; i++) {
//             char encrypted_char = caesar_cipher(buffer[i]);
//             usart_send(encrypted_char);
//         }
        
//         // Send carriage return and newline to complete the response
//         usart_send('\r');
//         usart_send('\n');
        
//         // Reset buffer index for the next sentence
//         index = 0;
//     }

//     return 0;
// }