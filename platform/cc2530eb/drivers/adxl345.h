#ifndef ADXL345_H_
#define ADXL345_H_

#include "cc253x.h"

typedef struct adxl345_data{
    int x;
    int y;
    int z;
}ADXL345_DATA;

extern void Init_ADXL345(void);

extern void Multiple_Read_ADXL345(ADXL345_DATA *dat);

#endif
