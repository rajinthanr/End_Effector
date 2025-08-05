#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "main.h"

void process_command(const char* cmd);
void pick_box();
void drop_box();
void lift(int state);
void grip(int state);
void vacuum(int state);


#endif