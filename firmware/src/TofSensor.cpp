
#include <math.h>
#include "TofSensor.h"
#include "uart.h"

extern UART uart0;
extern UART uart1;

uint8_t queryCommand[8] = {0x57, 0x10, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0x66};
uint8_t frame[FRAME_SIZE];
float distances[4];


constexpr float DEG2RAD = 3.14159265 / 180.0;


constexpr float H_FOV_DEG = 45.0;
constexpr float V_FOV_DEG = 45.0;


void tof_init()
{
    uart1.init(); // Initialize UART1
}

void read_tof()
{
    uart0.transmit("\n\n");

    sendQuery(); // Send the query command to the sensor
    
    // Wait for header
    while (true)
    {
        if (uart1.read() == HEADER_BYTE_0)
        {
            uart1.read(); 
            break;
        }
    }
    

    // Read the rest of the frame (FRAME_SIZE - 2 already read)
    int bytesRead = 0;
    while (bytesRead < FRAME_SIZE - 2)
    {
        if(uart1.available())
        {
            frame[bytesRead++] = uart1.read();
        }
    }
    //uart0.transmit("Frame received\n");

    // Parse and print corner distances 
    int dataStart = 7; 
    int distances_index = 0;
    for (int i = 0; i < DATA_POINTS; i++)
    {
        int offset = dataStart + i * POINT_SIZE;
        if (offset + 2 >= FRAME_SIZE)
            break;

        float distance = calculateValue(frame[offset], frame[offset + 1], frame[offset + 2]);

        // Only print four corners
        //if (i == 35 || i == 36 || i == 43 || i == 44)
        if (i == 0 || i == 7 || i == 56 || i == 63)
        {
            printDistance(i, distance);
            distances[distances_index] = distance;
            distances_index++;
        }
    }

    calculateAngleFromCorners(distances[0], distances[1], distances[2], distances[3]);

    //_delay_ms(2000); 
}







float calculateAngleFromCorners(float d0, float d7, float d56, float d63)
{
    // Pixel grid positions for the 4 corners
    int x0 = 0, y0 = 0;
    int x7 = 7, y7 = 0;
    int x56 = 0, y56 = 7;
    int x63 = 7, y63 = 7;

    // Angle per pixel
    float hAnglePerPixel = H_FOV_DEG / 7.0;
    float vAnglePerPixel = V_FOV_DEG / 7.0;

    // Convert pixel positions to angles (centered at 0)
    float ax0 = (x0 - 3.5) * hAnglePerPixel;
    float ay0 = (y0 - 3.5) * vAnglePerPixel;
    float ax7 = (x7 - 3.5) * hAnglePerPixel;
    float ay7 = (y7 - 3.5) * vAnglePerPixel;
    float ax56 = (x56 - 3.5) * hAnglePerPixel;
    float ay56 = (y56 - 3.5) * vAnglePerPixel;
    float ax63 = (x63 - 3.5) * hAnglePerPixel;
    float ay63 = (y63 - 3.5) * vAnglePerPixel;

    // Convert to 3D coordinates
    float x_sum = d0 * tan(ax0 * DEG2RAD) +
                  d7 * tan(ax7 * DEG2RAD) +
                  d56 * tan(ax56 * DEG2RAD) +
                  d63 * tan(ax63 * DEG2RAD);

    float y_sum = d0 * tan(ay0 * DEG2RAD) +
                  d7 * tan(ay7 * DEG2RAD) +
                  d56 * tan(ay56 * DEG2RAD) +
                  d63 * tan(ay63 * DEG2RAD);

    float z_sum = d0 + d7 + d56 + d63;

    // Average to get direction vector
    float x_avg = x_sum / 4.0;
    float y_avg = y_sum / 4.0;
    float z_avg = z_sum / 4.0;

    // Compute angle from center in degrees
    float horizontalAngle = 10*atan2(x_avg, z_avg) * 180.0 / 3.14159265;
    float verticalAngle = 10*atan2(y_avg, z_avg) * 180.0 / 3.14159265;

    uart0.transmit("Horizontal Angle: ");
    uart0.transmit(horizontalAngle, 2);
    uart0.transmit(" Vertical Angle: ");
    uart0.transmit(verticalAngle, 2);
    uart0.transmit("\n");

   
    return horizontalAngle; 
}

// Parse int24 as per protocol: (b0 << 8 | b1 << 16 | b2 << 24) / 256
int32_t parse_int24(uint8_t b0, uint8_t b1, uint8_t b2)
{
    int32_t value = ((int32_t)b2 << 16) | ((int32_t)b1 << 8) | b0;
    // Sign extend if the top bit is set (for 24-bit signed)
    if (value & 0x800000)
        value |= 0xFF000000;
    return value;
}

float calculateValue(uint8_t b0, uint8_t b1, uint8_t b2)
{
    int32_t temp = parse_int24(b0, b1, b2);
    return temp / 1000.0; //mm 
}

void printDistance(int index, float raw)
{
    float mm = raw;
    uart0.transmit("Index ");
    uart0.transmit(" ");
    //uart0.transmit((float)index, 0);
    uart0.transmit(": ");
    uart0.transmit(mm, 3);
    uart0.transmit(" mm\n");
}

void sendQuery()
{
    uart1.transmit(queryCommand, 8);
}





