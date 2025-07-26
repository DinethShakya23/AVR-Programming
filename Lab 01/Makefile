MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

TARGET = main
SRC = main.c

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	avrdude -c arduino -p m328p -P COM5 -b 115200 -U flash:w:$(TARGET).hex

clean:
	del /Q *.hex *.elf