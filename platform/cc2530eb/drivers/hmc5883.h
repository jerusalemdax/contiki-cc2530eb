#ifndef HMC5883_H
#define HMC5883_H

#define HMC5883_SLAVE_ADDR 0x3C

typedef struct hmc5883_data{
    int x;
    int y;
    int z;
}HMC5883_DATA;


void Init_HMC5883(void);
void Multiple_Read_HMC5883(HMC5883_DATA *dat);
#endif
