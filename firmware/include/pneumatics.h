#ifndef __PNEUMATICS_H_
#define __PNEUMATICS_H_

#include <inttypes.h>

void init_pneumatics();
void activate_solenoid(uint8_t solenoid);
void deactivate_solenoid(uint8_t solenoid);

#endif 