#include <avr/io.h>
#include <avr/interrupt.h>
#include "millis.h"

volatile uint32_t millis_counter = 0;

void millis_init()
{
    // Set up Timer0 to overflow every 1ms
    TCCR0A = 0x00;                      // Normal mode
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    TIMSK0 = (1 << TOIE0);              // Enable overflow interrupt
    TCNT0 = 0;
    sei(); // Enable global interrupts
}

ISR(TIMER0_OVF_vect)
{
    // 16MHz / 64 = 250kHz → 256 ticks = 1.024ms ≈ 1ms
    millis_counter++;
}

uint32_t millis()
{
    uint32_t m;
    cli();
    m = millis_counter;
    sei();
    return m;
}
