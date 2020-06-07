#ifndef __SCCB_H
#define __SCCB_H
#include "drivers.h"
//#include "driver_misc.h"
#include "sys.h"

//IO��������	 
#define SCCB_SCL    		PEout(2)//дOV_SCL
#define SCCB_SDA    		PEout(3)//дOV_SDA
#define SCCB_READ_SDA   PEin(3) //��OV_SDA  
#define SCCB_ID   			0X60  			//OV2640��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif



