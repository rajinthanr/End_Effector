#ifndef __PERIPHERALS_H_
#define __PERIPHERALS_H_

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>

void setup_seven_segment();
void seven_segment(int num = 0);

#endif