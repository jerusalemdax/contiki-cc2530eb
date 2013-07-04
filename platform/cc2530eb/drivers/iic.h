#ifndef IIC_H
#define IIC_H
#include "cc253x.h"
#include <stdint.h>

#define SCL     P1_2
#define SDA     P1_3

#define SCLDirOut() P1DIR|=0x04
#define SDADirOut() P1DIR|=0x08
#define SDADirIn()  P1DIR&=~0x08

extern void I2C_Start(void);

extern void I2C_Stop(void);

extern void Sendack(uint8_t h);

extern uint8_t I2C_Check_ack(void);

extern void I2C_Write_Byte(uint8_t b);

extern uint8_t I2C_Read_Byte(void);

extern void I2C_Write(uint8_t Slave_Addr, uint8_t REG_Address,uint8_t REG_data);

extern uint8_t I2C_Read(uint8_t slave_address, uint8_t REG_Address);

#endif
