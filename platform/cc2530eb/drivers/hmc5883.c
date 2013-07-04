#include "iic.h"
#include "hmc5883.h"

void Init_HMC5883(void)
{
    SCLDirOut();
    SDADirOut();
    I2C_Write(HMC5883_SLAVE_ADDR, 0x02, 0x00);
}

void Multiple_Read_HMC5883(HMC5883_DATA *dat)
{
    uint8_t f;
    I2C_Start();
    I2C_Write_Byte(HMC5883_SLAVE_ADDR);
    I2C_Write_Byte(0x03);
    I2C_Start();
    I2C_Write_Byte(HMC5883_SLAVE_ADDR+1);
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
