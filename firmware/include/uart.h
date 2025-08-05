#ifndef __UART_H_
#define __UART_H_

#include"main.h"
#include <avr/io.h>
#include <String.h>
#include <inttypes.h>

#define CMD_BUFFER_SIZE 32

extern char uart_buffer[CMD_BUFFER_SIZE];
extern volatile uint8_t uart_index;
extern volatile bool cmd_ready;

class UART{
private:
    
    uint32_t baud_rate = 115200; 
    uint8_t ubrr_value = 16; 
    // UART registers 
    volatile uint8_t *ubrrh;
    volatile uint8_t *ubrrl;
    volatile uint8_t *ucsra;
    volatile uint8_t *ucsrb;
    volatile uint8_t *ucsrc;
    volatile uint8_t *udr;
    uint32_t en_port = 0; // Port for enable pin
    uint32_t en_pin = 0; // Pin for enable pin
    bool enable_pin = false; // Flag to indicate if enable pin is used
    

public:
    UART(volatile uint8_t *ubrrh,volatile uint8_t *ubrrl,volatile uint8_t *ucsra,volatile uint8_t *ucsrb,volatile uint8_t *ucsrc,volatile uint8_t *udr);
    UART(volatile uint8_t *ubrrh,volatile uint8_t *ubrrl,volatile uint8_t *ucsra,volatile uint8_t *ucsrb,volatile uint8_t *ucsrc,volatile uint8_t *udr, volatile uint8_t en_port, uint8_t en_pin);
    
    // Initialization function
    void init();
    void transmit(uint8_t data);
    void transmit(const char *str);
    void transmitv(long value);
    uint8_t read();
    bool available();
    void transmit(float value, int precision);
    void transmit(uint8_t *data, uint8_t size);
    bool receive_line(char *buffer, size_t size);
};

#endif // __UART_H_