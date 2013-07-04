#include "sfr-bits.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "cc253x.h"
#include "sys/energest.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <math.h>
#include "adxl345.h"
#include "iic.h"

#define	ADXL345_SLAVE_ADDR 0xA6

void Init_ADXL345(void)
{
    SCLDirOut();
    SDADirOut();
    I2C_Write(ADXL345_SLAVE_ADDR, 0x31,0x0B);
    I2C_Write(ADXL345_SLAVE_ADDR, 0x2C,0x0F);
    I2C_Write(ADXL345_SLAVE_ADDR, 0x2D,0x08);
    I2C_Write(ADXL345_SLAVE_ADDR, 0x2E,0x80);
    I2C_Write(ADXL345_SLAVE_ADDR, 0x1E,0x00);
    I2C_Write(ADXL345_SLAVE_ADDR, 0x1F,0x00);
    I2C_Write(ADXL345_SLAVE_ADDR, 0x20,0x05);
}

void Multiple_Read_ADXL345(ADXL345_DATA *dat)
{
    uint8_t f;
    I2C_Start();
    I2C_Write_Byte(ADXL345_SLAVE_ADDR);
    I2C_Write_Byte(0x32);
    I2C_Start();
    I2C_Write_Byte(ADXL345_SLAVE_ADDR+1);
    for (f=0; f<6; f++)
    {
        ((uint8_t *)dat)[f] = I2C_Read_Byte();
        if (f == 5)
        {
            Sendack(1);
        }
        else
        {
            Sendack(0);
        }
    }
    I2C_Stop();
}
