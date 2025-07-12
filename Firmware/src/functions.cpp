#include "functions.h"
#include "main.h"
#include "fork.h"
#include "peripherals.h"
#include "uart.h"
#include "pneumatics.h"

#define LIFT_SOLENOID_1 3
#define LIFT_SOLENOID_2 4
#define GRIP_SOLENOID_1 2
#define GRIP_SOLENOID_2 1
#define VACUUM_SOLENOID 0

extern UART uart0;

void process_command(const char *cmd)
{
  if (strncmp(cmd, "lift=", 5) == 0)
  {
    int state = atoi(cmd + 5);
    lift(state);
    if (state == 0)
    {
      seven_segment(0);
    }
    else if (state == 1)
    {
      seven_segment(1);
    }
  }
  else if (strncmp(cmd, "grip=", 5) == 0)
  {
    int state = atoi(cmd + 5);
    grip(state);
    if (state == 0)
    {
      seven_segment(2);
    }
    else if (state == 1)
    {
      seven_segment(3);
    }
    else if (state == 2)
    {
      seven_segment(4);
    }
  }
  if (strncmp(cmd, "vacu=", 5) == 0)
  {
    int state = atoi(cmd + 5);
    vacuum(state);
    if (state == 0)
    {
      seven_segment(5);
    }
    else if (state == 1)
    {
      seven_segment(6);
    }
  }
  if (strncmp(cmd, "fork=", 5) == 0)
  {
    int value = atoi(cmd + 5);
    uart0.transmitv(value);
    drive_stepper(value);
  }

  if (strncmp(cmd, "read_tof", 8) == 0)
  {
    read_tof();
  }
}


//activate lift
void lift(int state)
{
  if (state == 0)
  {
    activate_solenoid(LIFT_SOLENOID_1);
    deactivate_solenoid(LIFT_SOLENOID_2);
  }
  else if (state == 1)
  {
    activate_solenoid(LIFT_SOLENOID_2);
    deactivate_solenoid(LIFT_SOLENOID_1);
  }
  else if (state == 2)
  {
    deactivate_solenoid(LIFT_SOLENOID_1);
    deactivate_solenoid(LIFT_SOLENOID_2);
  }
}


//activate grip
void grip(int state)
{
  if (state == 0)
  {
    activate_solenoid(GRIP_SOLENOID_1);
    deactivate_solenoid(GRIP_SOLENOID_2);
  }
  else if (state == 1)
  {
    activate_solenoid(GRIP_SOLENOID_2);
    deactivate_solenoid(GRIP_SOLENOID_1);
  }
  else if (state == 2)
  {
    deactivate_solenoid(GRIP_SOLENOID_1);
    deactivate_solenoid(GRIP_SOLENOID_2);
  }
}


//activate vacuum
void vacuum(int state)
{
  if (state == 0)
  {
    activate_solenoid(VACUUM_SOLENOID); // Activate vacuum
  }
  else if (state == 1)
  {
    deactivate_solenoid(VACUUM_SOLENOID); // Deactivate vacuum
  }
}

//funtion to pick up the box
void pick_box()
{
  vacuum(1);
  _delay_ms(1000); 
  // move closer to the box
  lift(1);
  _delay_ms(1000);   
  drive_stepper(10); // activate bottom rail
  lift(0);
  _delay_ms(1000); 
  vacuum(0);
  _delay_ms(1000); 
  grip(1);
  _delay_ms(1000); 
                   // move away from the box
}

void drop_box()
{
  grip(0);
  _delay_ms(1000);    
  drive_stepper(-10); // retract bottom rail
}