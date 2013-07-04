#ifndef L3G4200D_H
#define L3G4200D_H
#include "cc253x.h"
#include <stdint.h>

#define L3G4200D_SLAVE_ADDR 0xD2
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_TEMP 0x26
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

typedef struct l3g4200d_data{
    int x;
    int y;
    int z;
}L3G4200D_DATA;

void Init_L3G4200D(void);

void Multiple_Read_L3G4200D(L3G4200D_DATA *dat);

#endif
