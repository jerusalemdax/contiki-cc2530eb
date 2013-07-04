#include "l3g4200d.h"
#include "sys/clock.h"
#include "iic.h"

void Init_L3G4200D(void)
{
    SCLDirOut();
    SDADirOut();
    I2C_Write(L3G4200D_SLAVE_ADDR, CTRL_REG1,0x4f);
    I2C_Write(L3G4200D_SLAVE_ADDR, CTRL_REG2,0x00);
    I2C_Write(L3G4200D_SLAVE_ADDR, CTRL_REG3,0x08);
    I2C_Write(L3G4200D_SLAVE_ADDR, CTRL_REG4,0x00);
    I2C_Write(L3G4200D_SLAVE_ADDR, CTRL_REG5,0x00);
}

void Multiple_Read_L3G4200D(L3G4200D_DATA *dat)
{
    ((uint8_t *)dat)[0] = I2C_Read(L3G4200D_SLAVE_ADDR,OUT_X_L);
    ((uint8_t *)dat)[1] = I2C_Read(L3G4200D_SLAVE_ADDR,OUT_X_H);
    ((uint8_t *)dat)[2] = I2C_Read(L3G4200D_SLAVE_ADDR,OUT_Y_L);
    ((uint8_t *)dat)[3] = I2C_Read(L3G4200D_SLAVE_ADDR,OUT_Y_H);
    ((uint8_t *)dat)[4] = I2C_Read(L3G4200D_SLAVE_ADDR,OUT_Z_L);
    ((uint8_t *)dat)[5] = I2C_Read(L3G4200D_SLAVE_ADDR,OUT_Z_H);
}
