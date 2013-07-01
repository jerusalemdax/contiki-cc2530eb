#ifndef ADXL345_H_
#define ADXL345_H_

#include "cc253x.h"

typedef struct adxl345_data{
    int x;
    int y;
    int z;
}ADXL345_DATA;

extern uint8_t POS_TEMP[6];

extern void Init_ADXL345();

extern void Multiple_Read_ADXL345(ADXL345_DATA *dat);

#endif
