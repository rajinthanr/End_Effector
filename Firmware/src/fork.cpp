#include "fork.h"
#include "main.h"

//intialize fork
void init_fork()
{
  DDRH |= 0b01111000; 
  DDRE |= 1 << 3;
  PORTH &= ~(1 << 4);
}

void drive_stepper(int distance_cm)
{
  const int steps_per_cm = 10000; 
  int steps = abs(distance_cm) * steps_per_cm;
  bool direction = (distance_cm > 0); // True for forward, false for backward

  // Set direction 
  if (direction)
    PORTH |= (1 << 3); // Forward
  else
    PORTH &= ~(1 << 3); // Backward

  PORTH &= ~(1 << 4);

  for (int i = 0; i < steps; i++)
  {
    // Generate pulse 
    PORTE |= (1 << 3);  
    _delay_us(10);      
    PORTE &= ~(1 << 3); 
    _delay_us(10);      
  }

  // Disable stepper motor
  PORTH |= (1 << 4);
}