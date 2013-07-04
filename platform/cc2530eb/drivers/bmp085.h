#ifndef BMP085_H
#define BMP085_H
#include "cc253x.h"
#include <stdint.h>

#define BMP085_SLAVE_ADDR 0xee
#define OSS 0
typedef struct bmp085_data{
    int16_t temp;
    int32_t press;
} BMP085_DATA;

void Init_BMP085(void);

void Multiple_Read_BMP085(BMP085_DATA *dat);
#endif
