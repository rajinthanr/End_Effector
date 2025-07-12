#include "pneumatics.h"
#include <inttypes.h>
#include "peripherals.h"

/*
-PB4 - top gripper D10
-PB5 - sucction lifter D11
-PB6 - suction gripper D12
-PB7 - reserved D13
*/
void init_pneumatics()
{
  DDRB |= 0b11110000;     // Set PB4-PB7 solenoids as output
  PORTB &= ~(0b11110000); // Set PB4-PB7 Set solenoids low
  // PORTB |= (0b11110000);
  DDRE |= (1 << 4);   // Set PE4 solenoid as output
  PORTE &= ~(1 << 4); 
}

void activate_solenoid(uint8_t solenoid)
{
 switch (solenoid)
  {
  case 0:              // Top gripper
    PORTB |= (1 << 4); 
    break;
  case 1:              // Suction lifter
    PORTB |= (1 << 5); 
    break;
  case 2:              // Suction gripper
    PORTB |= (1 << 6); 
    break;
  case 3:              // Reserved
    PORTB |= (1 << 7); 
    break;
  case 4:              // Additional solenoid on PE4
    PORTE |= (1 << 4); 
    break;
  default:
    break; // Invalid solenoid number
  }
  }

void deactivate_solenoid(uint8_t solenoid)
{
  switch (solenoid)
  {
  case 0:               // Top gripper
    PORTB &= ~(1 << 4); 
    break;
  case 1:               // Suction lifter
    PORTB &= ~(1 << 5); 
    break;
  case 2:               // Suction gripper
    PORTB &= ~(1 << 6); 
    break;
  case 3:               // Reserved
    PORTB &= ~(1 << 7); 
    break;
  case 4:               // Additional solenoid on PE4
    PORTE &= ~(1 << 4); 
    break;
  default:
    break; // Invalid solenoid number
  }
}