#ifndef TOFSENSOR_H
#define TOFSENSOR_H


#define FRAME_SIZE 400
#define DATA_POINTS 64
#define POINT_SIZE 6
#define HEADER_BYTE_0 0x57
#define HEADER_BYTE_1 0x01

#include <math.h>
#include <stdint.h>
#include "main.h"
//#include <Arduino.h>

// Variable prototypes
extern uint8_t queryCommand[8];
extern uint8_t frame[FRAME_SIZE];
extern float distances[4];

float calculateAngleFromCorners(float d0, float d7, float d56, float d63);
int32_t parse_int24(uint8_t b0, uint8_t b1, uint8_t b2);
float calculateValue(uint8_t b0, uint8_t b1, uint8_t b2);
void printDistance(int index, float raw);
void sendQuery();
void tof_init();
void read_tof();
void UART0Init();
void UART1Init();
void UART0Transmit(const char *data);
void UART0Transmit(uint8_t data);
void UART0Transmit(uint8_t *data, uint8_t size);
void UART0Transmit(float value, int precision);
void UART1Transmit(const char *data);
void UART1Transmit(uint8_t data);
void UART1Transmit(uint8_t *data, uint8_t size);
void UART0Transmitc(char data);
uint8_t UART1Read();
bool UART1Available();

#endif 