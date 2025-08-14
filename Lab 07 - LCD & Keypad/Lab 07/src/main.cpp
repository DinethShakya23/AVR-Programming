// //Lab 07
// //Exercise 01
// #include <avr/io.h>
// #include <util/delay.h>

// //#define F_CPU 16000000UL  // Define CPU frequency for delay functions
// #define BAUD 9600          // Define Baud rate for UART
// #define MYUBRR (F_CPU/16/BAUD - 1)  // Calculate UBRR value for the desired baud rate

// // Function to initialize UART communication
// void UART_init(unsigned int ubrr) {
//     UBRR0 = ubrr;  // Set baud rate
//     UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable receiver and transmitter
//     UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set data frame: 8 data bits, 1 stop bit
// }

// // Function to send a single character over UART
// void UART_send(char data) {
//     while (!(UCSR0A & (1 << UDRE0))); // Wait until transmit buffer is empty
//     UDR0 = data; // Send the data
// }

// // Function to receive a single character over UART
// char UART_receive() {
//     while (!(UCSR0A & (1 << RXC0))); // Wait until data is received
//     return UDR0; // Return received data
// }

// // Function to write a byte to EEPROM at a given address
// void EEPROMwrite(unsigned int address, unsigned char data) {
//     while (EECR & (1 << EEPE)); // Wait for completion of previous write
//     EEAR = address; // Set EEPROM address
//     EEDR = data;    // Put data in EEPROM data register
//     EECR |= (1 << EEMPE); // Enable master write
//     EECR |= (1 << EEPE);  // Start EEPROM write
// }

// // Function to read a byte from EEPROM at a given address
// unsigned char EEPROMread(unsigned int address) {
//     while (EECR & (1 << EEPE)); // Wait for completion of previous write
//     EEAR = address; // Set EEPROM address
//     EECR |= (1 << EERE); // Start EEPROM read
//     return EEDR;    // Return the read data
// }

// // Function to receive a sentence via UART and store it in EEPROM
// void write_sentence_to_eeprom() {
//     unsigned int addr = 0; // EEPROM address counter
//     char c;
//     do {
//         c = UART_receive();    // Receive a character
//         EEPROMwrite(addr++, c); // Write character to EEPROM
//     } while (c != '\r' && addr < 1024); // Continue until carriage return or EEPROM limit
// }

// // Function to read stored sentence from EEPROM and send it via UART
// void read_eeprom_and_print() {
//     unsigned int addr = 0; // EEPROM address counter
//     char c;
//     do {
//         c = EEPROMread(addr++); // Read character from EEPROM
//         UART_send(c);           // Send character over UART
//     } while (c != '\r' && addr < 1024); // Continue until carriage return or EEPROM limit
// }


// int main() {
//     UART_init(MYUBRR);

//     // Prompt user
//     char* msg = "Enter ASCII text ending with \\r:\n";
//     for (int i = 0; msg[i]; i++) UART_send(msg[i]);

//     // Write to EEPROM
//     write_sentence_to_eeprom();

//     // Print EEPROM content
//     char* msg2 = "\nEEPROM Contents:\n";
//     for (int i = 0; msg2[i]; i++) UART_send(msg2[i]);

//     read_eeprom_and_print();

//     while (1);
// }



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// === LCD Pins connected to PORTB ===
#define LCD_RS PB0
#define LCD_E PB1
#define LCD_D4 PB2
#define LCD_D5 PB3
#define LCD_D6 PB4
#define LCD_D7 PB5

// === Keypad configuration ===
const char KEYPAD[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

#define KEY_ADDR 0

// Helper macros to set and clear bits in registers
#define SET_BIT(port, bit) ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))


// -------- LCD FUNCTIONS --------
void lcd_enable()
{
    SET_BIT(PORTB, LCD_E);
    _delay_us(1);
    CLEAR_BIT(PORTB, LCD_E);
    _delay_us(100);
}

// Send 4-bit nibble to LCD data pins (D4-D7)
void lcd_send_nibble(uint8_t nibble)
{
    if (nibble & 0x01)
        SET_BIT(PORTB, LCD_D4);
    else
        CLEAR_BIT(PORTB, LCD_D4);

    if (nibble & 0x02)
        SET_BIT(PORTB, LCD_D5);
    else
        CLEAR_BIT(PORTB, LCD_D5);

    if (nibble & 0x04)
        SET_BIT(PORTB, LCD_D6);
    else
        CLEAR_BIT(PORTB, LCD_D6);

    if (nibble & 0x08)
        SET_BIT(PORTB, LCD_D7);
    else
        CLEAR_BIT(PORTB, LCD_D7);

    lcd_enable();
}

// Send byte to LCD: is_data=1 for data, 0 for command
void lcd_send_byte(uint8_t byte, uint8_t is_data)
{
    if (is_data)
        SET_BIT(PORTB, LCD_RS);
    else
        CLEAR_BIT(PORTB, LCD_RS);

    lcd_send_nibble(byte >> 4);
    lcd_send_nibble(byte & 0x0F);
    _delay_ms(2);
}

// Send command byte to LCD
void lcd_cmd(uint8_t cmd)
{
    lcd_send_byte(cmd, 0);
}

// Write a single character to LCD
void lcd_char(char c)
{
    lcd_send_byte(c, 1);
}

// Write null-terminated string to LCD
void lcd_print(const char *str)
{
    while (*str)
        lcd_char(*str++);
}

// Initialize LCD in 4-bit mode, 2 lines, no cursor
void lcd_init()
{
    DDRB |= 0x3F; 

    _delay_ms(50); 

    
    lcd_send_nibble(0x03);
    _delay_ms(5);
    lcd_send_nibble(0x03);
    _delay_ms(5);
    lcd_send_nibble(0x03);
    _delay_ms(1);
    lcd_send_nibble(0x02); 

    lcd_cmd(0x28); 
    lcd_cmd(0x0C); 
    lcd_cmd(0x06); 
    lcd_cmd(0x01); 
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    lcd_cmd(0x80 + (row * 0x40) + col);
}

// -------- EEPROM FUNCTIONS --------
void EEPROMwrite(uint16_t address, uint8_t data)
{
    while (EECR & (1 << EEPE))
        ;                 
    cli();              
    EEAR = address;      
    EEDR = data;       
    EECR |= (1 << EEMPE); 
    EECR |= (1 << EEPE);  
    sei();                
}

// Read a byte from EEPROM address
uint8_t EEPROMread(uint16_t address)
{
    while (EECR & (1 << EEPE))
        ;                
    EEAR = address;     
    EECR |= (1 << EERE); 
    return EEDR;        
}

// -------- CAESAR CIPHER FUNCTION --------


char caesar(char c, uint8_t key)
{
    if (c >= 'A' && c <= 'Z')
        return ((c - 'A' + key) % 26) + 'A';
    if (c >= 'a' && c <= 'z')
        return ((c - 'a' + key) % 26) + 'a';
    return c; // Non-alphabetic characters unchanged
}

// -------- KEYPAD FUNCTIONS --------


void keypad_init()
{
    DDRC &= ~(0x0F); 
    PORTC |= 0x0F;   

    DDRD |= 0xF0;  
    PORTD |= 0xF0; 
}

// Scan keypad and return pressed key or 0 if none
char keypad_get_key()
{
    uint8_t col;
    for (col = 0; col < 4; col++)
    {
        PORTD = ~(1 << (col + 4)); 
        _delay_us(5);              

        uint8_t row;
        for (row = 0; row < 4; row++)
        {
            if (!(PINC & (1 << row))) // Check if row is LOW (pressed)
            {
                
                while (!(PINC & (1 << row)))
                    ;
                _delay_ms(50); 
                return KEYPAD[row][col];
            }
        }
    }
    return 0; // No key pressed
}

// Get exactly 'len' characters input from keypad, echo to LCD
void get_text(char *buffer, uint8_t len)
{
    uint8_t i = 0;
    while (i < len)
    {
        char key = keypad_get_key();
        if (key)
        {
            buffer[i++] = key;
            lcd_char(key);
        }
    }
    buffer[len] = '\0'; 
}


uint8_t input_key()
{
    uint8_t val = 0;
    while (1)
    {
        char key = keypad_get_key();
        if (key >= '0' && key <= '9')
        {
            val = val * 10 + (key - '0');
            lcd_char(key);
        }
        else if (key == '*')
        {
            return val;
        }
    }
}

// -------- MAIN PROGRAM --------

int main(void)
{
    lcd_init();    
    keypad_init(); 

    // Read stored key from EEPROM, default to 3 if invalid
    uint8_t key = EEPROMread(KEY_ADDR);
    if (key > 25)
    {
        key = 3;
        EEPROMwrite(KEY_ADDR, key);
    }

    while (1)
    {
        lcd_cmd(0x01); 
        lcd_print("1:Encrypt 2:Key");
        _delay_ms(1000);

        char choice = 0;
        while (!choice)
            choice = keypad_get_key();

        if (choice == '1') 
        {
            lcd_cmd(0x01);
            lcd_print("Enter text (10):");
            lcd_set_cursor(0, 1);

            char input[11], output[11];
            get_text(input, 10); 

            uint8_t i;
            for (i = 0; i < 10; i++)
                output[i] = caesar(input[i], key);
            output[10] = '\0';

            lcd_cmd(0x01);
            lcd_print("Encrypted:");
            lcd_set_cursor(0, 1);
            lcd_print(output);

            _delay_ms(40000); 
        }
        else if (choice == '2') 
        {
            lcd_cmd(0x01);
            lcd_print("Enter key 0-25:");
            lcd_set_cursor(0, 1);

            uint8_t newKey = input_key();

            if (newKey <= 25)
            {
                EEPROMwrite(KEY_ADDR, newKey);
                key = newKey;

                lcd_cmd(0x01);
                lcd_print("Key Saved!");
            }
            else
            {
                lcd_cmd(0x01);
                lcd_print("Invalid Key");
            }
            _delay_ms(3000);
        }
    }
}
