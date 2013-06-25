#ifndef ADXL345_H_
#define ADXL345_H_

/* #include  "ioCC2430.h" */
/* #include "OnBoard.h" */
/* #include "hal_lcd.h" */
/* #include "AXD.h" */

typedef unsigned char INT8U;
typedef unsigned int  INT16U;
typedef signed char   INT8S;
typedef signed int    INT16S;

//#define SCL P1_3      //IIC时钟引脚定义
//#define SDA P1_2      //IIC数据引脚定义

#define	SLAVE_ADDRESS   0xA6	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A
#define BYTE unsigned char
#define WORD unsigned int

extern INT8U POS_TEMP[6];

extern void Init_ADXL345(void);
extern void ADXL345_Start(void);
extern void ADXL345_Stop(void);
extern void ADXL345_SendACK(INT8U ack);
extern INT8U ADXL345_RecvACK(void);
extern void ADXL345_SendByte(BYTE dat);
extern BYTE ADXL345_RecvByte(void);
extern void ADXL345_ReadPage(void);
extern void ADXL345_WritePage(void);
extern void Multiple_Read_ADXL345(void);
/* void conversion(SEND_DATA *da, ACCELERATION * ac); */
extern void displayXYZ(INT8U *pData);
extern INT8U Single_Read_ADXL345(INT8U REG_Address);
#endif
