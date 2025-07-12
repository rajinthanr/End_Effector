#include "uart.h"
#include "main.h"
#include "inttypes.h"
#include "avr/io.h"
#include "millis.h"


UART::UART(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl, volatile uint8_t *ucsra, volatile uint8_t *ucsrb, volatile uint8_t *ucsrc, volatile uint8_t *udr)
    : ubrrh(ubrrh), ubrrl(ubrrl), ucsra(ucsra), ucsrb(ucsrb), ucsrc(ucsrc), udr(udr)
{
    // Constructor code if needed
}

UART::UART(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl, volatile uint8_t *ucsra, volatile uint8_t *ucsrb, volatile uint8_t *ucsrc, volatile uint8_t *udr, volatile uint8_t en_port, uint8_t en_pin)
    : ubrrh(ubrrh), ubrrl(ubrrl), ucsra(ucsra), ucsrb(ucsrb), ucsrc(ucsrc), udr(udr), en_port(en_port), en_pin(en_pin)
{
    enable_pin = true; // Set flag to indicate enable pin is used
}

void UART::init()
{
    // Set baud rate
    *ubrrh = (ubrr_value >> 8) & 0xFF;
    *ubrrl = ubrr_value & 0xFF;

    if (enable_pin)
    {
        DDRE |= (1 << en_pin);  // Set enable pin as output
        PORTE |= (1 << en_pin); // Set enable pin high if used
    }
    // Set UCSRnA for single speed mode
    *ucsra |= (1 << U2X0); // Enable double speed mode if needed

    // Enable receiver and transmitter
    *ucsrb |= (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    *ucsrc |= (1 << UCSZ01) | (1 << UCSZ00);
}

void UART::transmit(uint8_t data)
{
    if (enable_pin)
    {
        PORTE |= (1 << en_pin);
    }
    // Wait for empty transmit buffer
    while (!(*ucsra & (1 << UDRE0)))
        ;
    // Put data into buffer, sends the data
    *udr = data;
}

void UART::transmit(const char *str)
{
    while (*str)
    {
        transmit(*str);
        str++;
    }
}

void UART::transmitv(long value)
{
    char buffer[20];
    ltoa(value, buffer, 10);
    transmit(buffer);
}

uint8_t UART::read()
{
    // Wait for data to be received
    unsigned long start = millis();
    while (!available()) {
        if (millis() - start > 10) {
            return 0; // Timeout after 10ms, return 0 or handle as needed
        }
    }
        ;
    // Get and return received data from the buffer
    return *udr;
}

bool UART::available()
{
    static unsigned long count = 0;
    if (enable_pin)
    {
        if ((*ucsra & (1 << UDRE0)) && (*ucsra & (1 << TXC0)))
        {
            if (count > 1000)
                PORTE &= ~(1 << en_pin); // Set enable pin low before checking availability
            else
                count++;
        }
        else
        {
            count = 0;
        }
    }
    // Check if data is available in the receive buffer
    return (*ucsra & (1 << RXC0));
}

void UART::transmit(float value, int precision)
{
    // char buffer[32];
    // snprintf(buffer, sizeof(buffer), "%.*f", precision, (double)value);
    // transmit(buffer);
    char buffer[20];
    dtostrf(value, 1, precision, buffer);
    transmit(buffer);
}

void UART::transmit(uint8_t *data, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        transmit(data[i]);
    }
}

bool UART::receive_line(char *buffer, size_t size)
{
    if (!available())
        return false; // No data available

    size_t index = 0;
    while (index < size - 1)
    {
        char c = read();
        if (c == '\n' || c == '\r')
        {
            break; // End of line
        }
        buffer[index++] = c;
    }
    buffer[index] = '\0'; // Null-terminate the string
    return true;          // Successfully received a line
}
