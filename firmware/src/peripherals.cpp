#include "peripherals.h"
#include "main.h"
#include "TofSensor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <stdint.h>

void setup_seven_segment()
{
    DDRL = 0xFF; 
}

void seven_segment(int num)
{
    int number[] = {0b1110111, 0b0010010, 0b1011101,
                    0b1011011, 0b0111010, 0b1101011,
                    0b1101111, 0b1010010, 0b1111111,
                    0b1111011}; 

    int segment[] = {3, 2, 4, 6, 1, 5, 0}; // Segment mapping for seven-segment display

    PORTL = 0x00;
    PORTL &= ~(1 << 7);
    //_delay_ms(200);

    for (int i = 0; i < 7; i++)
    {
        if (number[num] & (1 << (6 - i)))
            PORTL |= (1 << segment[i]); // Set the corresponding segment high
                                        //_delay_ms(200);               
    }
}
