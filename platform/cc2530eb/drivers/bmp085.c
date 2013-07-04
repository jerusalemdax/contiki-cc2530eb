#include "bmp085.h"
#include "iic.h"
#include "sfr-bits.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "cc253x.h"
#include "sys/energest.h"
#include <stdio.h>
#include <stdint.h>
int16_t ac1;
int16_t ac2;
int16_t ac3;
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1;
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;

int16_t BMP085_Read_2B(uint8_t addr)
{
    uint8_t msb, lsb;
    I2C_Start();
    I2C_Write_Byte(BMP085_SLAVE_ADDR);
    I2C_Write_Byte(addr);
    I2C_Start();
    I2C_Write_Byte(BMP085_SLAVE_ADDR+1);
    msb=I2C_Read_Byte();
    Sendack(0);
    lsb=I2C_Read_Byte();
    Sendack(1);
    I2C_Stop();
    return (int16_t)((msb << 8) | lsb);
}

uint32_t BMP085_Read_3B(uint8_t addr)
{
    uint8_t msb, lsb, xlsb;
    uint32_t up = 0;
    I2C_Start();
    I2C_Write_Byte(BMP085_SLAVE_ADDR);
    I2C_Write_Byte(addr);
    I2C_Start();
    I2C_Write_Byte(BMP085_SLAVE_ADDR+1);
    msb=I2C_Read_Byte();
    Sendack(0);
    lsb=I2C_Read_Byte();
    Sendack(0);
    xlsb=I2C_Read_Byte();
    Sendack(1);
    I2C_Stop();
    return (((uint32_t) msb << 16) | ((uint32_t) lsb << 8) | (uint32_t) xlsb) >> (8-OSS);
}
int16_t BMP085_Read_TEMP(void)
{
    int16_t temp;
    I2C_Write(BMP085_SLAVE_ADDR, 0xF4, 0x2E);
    clock_delay_usec(5000);
    return (int16_t)BMP085_Read_2B(0xF6);
}

int32_t BMP085_Read_Pressure(void)
{
    long pressure;
    I2C_Write(BMP085_SLAVE_ADDR, 0xF4, (0x34 + (OSS << 6)));
    clock_delay_usec((2 + (3 << OSS)) * 1000);
    return ((int32_t)BMP085_Read_3B(0xF6));
}

void Init_BMP085(void)
{
    SCLDirOut();
    SDADirOut();
    ac1 = BMP085_Read_2B(0xAA);
    ac2 = BMP085_Read_2B(0xAC);
    ac3 = BMP085_Read_2B(0xAE);
    ac4 = BMP085_Read_2B(0xB0);
    ac5 = BMP085_Read_2B(0xB2);
    ac6 = BMP085_Read_2B(0xB4);
    b1 =  BMP085_Read_2B(0xB6);
    b2 =  BMP085_Read_2B(0xB8);
    mb =  BMP085_Read_2B(0xBA);
    mc =  BMP085_Read_2B(0xBC);
    md =  BMP085_Read_2B(0xBE);
}

void Multiple_Read_BMP085(BMP085_DATA *dat)
{
    int32_t ut;
    int32_t x1, x2, b5, b6, x3, b3, p;
    uint32_t b4, b7;
    dat->temp = BMP085_Read_TEMP();

    x1 = (((int32_t)dat->temp - ac6) * ac5) >> 15;
    x2 = ((int32_t)mc << 11) / (x1 + md);
    b5 = x1 + x2;
    dat->temp = ((b5 + 8) >> 4);

    dat->press = BMP085_Read_Pressure();
    b6 = b5 - 4000;
    x1 = (b2 * (b6 * b6) >> 12) >> 11;
    x2 = (ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = (((((int32_t)ac1) * 4 + x3) << OSS) + 2) >> 2;
    x1 = (ac3 * b6) >> 13;
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
    b7 = ((uint32_t)dat->press - b3) * (50000 >> OSS);
    if( b7 < 0x80000000)
        p = (b7 * 2) / b4 ;
    else
        p = (b7 / b4) * 2;
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p += ((x1 + x2 + 3791) >> 4);
    dat->press = p;
}
