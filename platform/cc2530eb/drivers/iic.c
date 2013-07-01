#include "iic.h"
#include "sys/clock.h"

void I2C_Start(void)
{
    SDADirOut;
    SDA=1;
    SCL=1;
    SDA=0;
    SCL=0;
}

void I2C_Stop(void)
{
    SCL=0;
    SDADirOut;
    SDA=0;
    SCL=1;
    SDA=1;
}

void Sendack(uint8_t h)
{
    SCL=0;
    SDADirOut;
    SDA=h&0x01;
    SCL=1;
    clock_delay_usec(1);
    SCL=0;
}

uint8_t I2C_Check_ack(void)
{
    SCL=0;
    SDADirOut;
    SDA=1;
    SDADirIn;
    SCL=1;
    if(SDA==1)
    {
        SCL=0;
        return 0;
    }
    SCL=0;
    return 1;
}

void I2C_Write_Byte(uint8_t b)
{
    uint8_t e=8;
    SDADirOut;
    while(e--){
        SCL=0;
        if(b&0x80)SDA=1;
        else SDA=0;
        b<<=1;
        SCL=1;
    }
    SCL=0;
    I2C_Check_ack();
}

uint8_t I2C_Read_Byte(void)
{
    uint8_t i=8;
    uint8_t c=0;
    SCL=0;
    SDADirOut;
    SDA=1;
    SDADirIn;
    while(i--){
        c<<=1;
        SCL=0;
        clock_delay_usec(1);
        SCL=1;
        if(SDA==1)c|=0x01;
        else c&=0xfe;
    }
    SCL=0;
    SDADirOut;
    return c;
}

void I2C_Write(uint8_t Slave_Addr, uint8_t REG_Address,uint8_t REG_data)
{
    I2C_Start();
    I2C_Write_Byte(Slave_Addr);
    I2C_Write_Byte(REG_Address);
    I2C_Write_Byte(REG_data);
    I2C_Stop();
}

uint8_t I2C_Read(uint8_t Slave_Addr, uint8_t REG_Address)
{
    uint8_t REG_data;
    I2C_Start();
    I2C_Write_Byte(Slave_Addr);
    I2C_Write_Byte(REG_Address);
    I2C_Start();
    I2C_Write_Byte(Slave_Addr+1);
    REG_data=I2C_Read_Byte();
    Sendack(1);
    I2C_Stop();
    return REG_data;
}
