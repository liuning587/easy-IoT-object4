#define DEBUG
#include "drivers.h"
//#include "ov2640_reg.h"
#include "ov5640af.h"
#include "ov2640_reg.h"
#include "debug.h"

extern void delay_ms(uint32_t us);
//OV2640��ʼ��
//u8 OV2640_Init(void)
//{
//	u8 i;
//	u16 reg;
//
//   u16 OV2640_ID;//��¼������ID���ƷID
//
//	GPIO_InitTypeDef  GPIO_InitStructure;
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOD,Eʱ��
//	//OV_RESET OV_PWDN ����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //���ģʽ
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//25MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//GPIOD6�������
//  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//GPIOE5�������
//  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
//	OV2640_PWDN=0;	//POWER ON
//	sleep(10);
//	OV2640_RST=0;	//��λOV2640
//	sleep(10);
//	OV2640_RST=1;	//������λ
//    SCCB_Init();        		//��ʼ��SCCB ��IO��
//	SCCB_WR_Reg(OV2640_DSP_RA_DLMT, 0x01);	//����sensor�Ĵ���
// 	SCCB_WR_Reg(OV2640_SENSOR_COM7, 0x80);	//��λOV2640
//	sleep(50);
//	reg=SCCB_RD_Reg(OV2640_SENSOR_MIDH);	//��ȡ����ID �߰�λ
//	reg<<=8;
//	reg|=SCCB_RD_Reg(OV2640_SENSOR_MIDL);	//��ȡ����ID �Ͱ�λ
//	if(reg!=OV2640_MID)
//	{
//		printf("MID:%4x\r\n",reg);
//		return 1;
//	}
//	reg=SCCB_RD_Reg(OV2640_SENSOR_PIDH);	//��ȡ����ID �߰�λ
//	reg<<=8;
//	reg|=SCCB_RD_Reg(OV2640_SENSOR_PIDL);	//��ȡ����ID �Ͱ�λ
//	if(reg!=OV2640_PID)
//	{
//		printf("HID:%4x\r\n",reg);
//		return 2;
//	}
// 	//��ʼ�� OV2640,����SXGA�ֱ���(1600*1200)
//	for(i=0;i<sizeof(ov2640_svga_init_reg_tbl)/2;i++)
//	{
//	   	SCCB_WR_Reg(ov2640_svga_init_reg_tbl[i][0],ov2640_svga_init_reg_tbl[i][1]);
// 	}
//  	return 0x00; 	//ok
//}


////OV2640�л�ΪJPEGģʽ
//void OV2640_JPEG_Mode(void)
//{
//	u16 i=0;
//	//����:YUV422��ʽ
//	for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
//	{
//		SCCB_WR_Reg(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]);
//	}
//
//	//����:���JPEG����
//	for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
//	{
//		SCCB_WR_Reg(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);
//	}
//}


////OV2640�л�ΪRGB565ģʽ
//void OV2640_RGB565_Mode(void)
//{
//	u16 i=0;
//	//����:RGB565���
//	for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
//	{
//		SCCB_WR_Reg(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]);
//	}
//}


////�Զ��ع����ò�����,֧��5���ȼ�
//const static u8 OV2640_AUTOEXPOSURE_LEVEL[5][8]=
//{
//	{
//		0xFF,0x01,
//		0x24,0x20,
//		0x25,0x18,
//		0x26,0x60,
//	},
//	{
//		0xFF,0x01,
//		0x24,0x34,
//		0x25,0x1c,
//		0x26,0x00,
//	},
//	{
//		0xFF,0x01,
//		0x24,0x3e,
//		0x25,0x38,
//		0x26,0x81,
//	},
//	{
//		0xFF,0x01,
//		0x24,0x48,
//		0x25,0x40,
//		0x26,0x81,
//	},
//	{
//		0xFF,0x01,
//		0x24,0x58,
//		0x25,0x50,
//		0x26,0x92,
//	},
//};
////OV2640�Զ��ع�ȼ�����
////level:0~4
//void OV2640_Auto_Exposure(u8 level)
//{
//	u8 i;
//	u8 *p=(u8*)OV2640_AUTOEXPOSURE_LEVEL[level];
//	for(i=0;i<4;i++)
//	{
//		SCCB_WR_Reg(p[i*2],p[i*2+1]);
//	}
//}
////��ƽ������
////0:�Զ�
////1:̫��sunny
////2,����cloudy
////3,�칫��office
////4,����home
//void OV2640_Light_Mode(u8 mode)
//{
//	u8 regccval=0X5E;//Sunny
//	u8 regcdval=0X41;
//	u8 regceval=0X54;
//	switch(mode)
//	{
//		case 0://auto
//			SCCB_WR_Reg(0XFF,0X00);
//			SCCB_WR_Reg(0XC7,0X10);//AWB ON
//			return;
//		case 2://cloudy
//			regccval=0X65;
//			regcdval=0X41;
//			regceval=0X4F;
//			break;
//		case 3://office
//			regccval=0X52;
//			regcdval=0X41;
//			regceval=0X66;
//			break;
//		case 4://home
//			regccval=0X42;
//			regcdval=0X3F;
//			regceval=0X71;
//			break;
//	}
//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XC7,0X40);	//AWB OFF
//	SCCB_WR_Reg(0XCC,regccval);
//	SCCB_WR_Reg(0XCD,regcdval);
//	SCCB_WR_Reg(0XCE,regceval);
//}
////ɫ������
////0:-2
////1:-1
////2,0
////3,+1
////4,+2
//void OV2640_Color_Saturation(u8 sat)
//{
//	u8 reg7dval=((sat+2)<<4)|0X08;
//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0X7C,0X00);
//	SCCB_WR_Reg(0X7D,0X02);
//	SCCB_WR_Reg(0X7C,0X03);
//	SCCB_WR_Reg(0X7D,reg7dval);
//	SCCB_WR_Reg(0X7D,reg7dval);
//}
////��������
////0:(0X00)-2
////1:(0X10)-1
////2,(0X20) 0
////3,(0X30)+1
////4,(0X40)+2
//void OV2640_Brightness(u8 bright)
//{
//  SCCB_WR_Reg(0xff, 0x00);
//  SCCB_WR_Reg(0x7c, 0x00);
//  SCCB_WR_Reg(0x7d, 0x04);
//  SCCB_WR_Reg(0x7c, 0x09);
//  SCCB_WR_Reg(0x7d, bright<<4);
//  SCCB_WR_Reg(0x7d, 0x00);
//}
////�Աȶ�����
////0:-2
////1:-1
////2,0
////3,+1
////4,+2
//void OV2640_Contrast(u8 contrast)
//{
//	u8 reg7d0val=0X20;//Ĭ��Ϊ��ͨģʽ
//	u8 reg7d1val=0X20;
//  	switch(contrast)
//	{
//		case 0://-2
//			reg7d0val=0X18;
//			reg7d1val=0X34;
//			break;
//		case 1://-1
//			reg7d0val=0X1C;
//			reg7d1val=0X2A;
//			break;
//		case 3://1
//			reg7d0val=0X24;
//			reg7d1val=0X16;
//			break;
//		case 4://2
//			reg7d0val=0X28;
//			reg7d1val=0X0C;
//			break;
//	}
//	SCCB_WR_Reg(0xff,0x00);
//	SCCB_WR_Reg(0x7c,0x00);
//	SCCB_WR_Reg(0x7d,0x04);
//	SCCB_WR_Reg(0x7c,0x07);
//	SCCB_WR_Reg(0x7d,0x20);
//	SCCB_WR_Reg(0x7d,reg7d0val);
//	SCCB_WR_Reg(0x7d,reg7d1val);
//	SCCB_WR_Reg(0x7d,0x06);
//}
////��Ч����
////0:��ͨģʽ
////1,��Ƭ
////2,�ڰ�
////3,ƫ��ɫ
////4,ƫ��ɫ
////5,ƫ��ɫ
////6,����
//void OV2640_Special_Effects(u8 eft)
//{
//	u8 reg7d0val=0X00;//Ĭ��Ϊ��ͨģʽ
//	u8 reg7d1val=0X80;
//	u8 reg7d2val=0X80;
//	switch(eft)
//	{
//		case 1://��Ƭ
//			reg7d0val=0X40;
//			break;
//		case 2://�ڰ�
//			reg7d0val=0X18;
//			break;
//		case 3://ƫ��ɫ
//			reg7d0val=0X18;
//			reg7d1val=0X40;
//			reg7d2val=0XC0;
//			break;
//		case 4://ƫ��ɫ
//			reg7d0val=0X18;
//			reg7d1val=0X40;
//			reg7d2val=0X40;
//			break;
//		case 5://ƫ��ɫ
//			reg7d0val=0X18;
//			reg7d1val=0XA0;
//			reg7d2val=0X40;
//			break;
//		case 6://����
//			reg7d0val=0X18;
//			reg7d1val=0X40;
//			reg7d2val=0XA6;
//			break;
//	}
//	SCCB_WR_Reg(0xff,0x00);
//	SCCB_WR_Reg(0x7c,0x00);
//	SCCB_WR_Reg(0x7d,reg7d0val);
//	SCCB_WR_Reg(0x7c,0x05);
//	SCCB_WR_Reg(0x7d,reg7d1val);
//	SCCB_WR_Reg(0x7d,reg7d2val);
//}
////��������
////sw:0,�رղ���
////   1,��������(ע��OV2640�Ĳ����ǵ�����ͼ�������)
//void OV2640_Color_Bar(u8 sw)
//{
//	u8 reg;
//	SCCB_WR_Reg(0XFF,0X01);
//	reg=SCCB_RD_Reg(0X12);
//	reg&=~(1<<1);
//	if(sw)reg|=1<<1;
//	SCCB_WR_Reg(0X12,reg);
//}
////����ͼ���������
////sx,sy,��ʼ��ַ
////width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
//void OV2640_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
//{
//	u16 endx;
//	u16 endy;
//	u8 temp;
//	endx=sx+width/2;	//V*2
// 	endy=sy+height/2;
//
//	SCCB_WR_Reg(0XFF,0X01);
//	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
//	temp&=0XF0;
//	temp|=((endy&0X03)<<2)|(sy&0X03);
//	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
//	SCCB_WR_Reg(0X19,sy>>2);			//����Vref��start��8λ
//	SCCB_WR_Reg(0X1A,endy>>2);			//����Vref��end�ĸ�8λ
//
//	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
//	temp&=0XC0;
//	temp|=((endx&0X07)<<3)|(sx&0X07);
//	SCCB_WR_Reg(0X32,temp);				//����Href��start��end�����3λ
//	SCCB_WR_Reg(0X17,sx>>3);			//����Href��start��8λ
//	SCCB_WR_Reg(0X18,endx>>3);			//����Href��end�ĸ�8λ
//}
////����ͼ�������С
////OV2640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸĺ���ȷ��
////width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
////����ֵ:0,���óɹ�
////    ����,����ʧ��
//u8 OV2640_OutSize_Set(u16 width,u16 height)
//{
//	u16 outh;
//	u16 outw;
//	u8 temp;
//	if(width%4)return 1;
//	if(height%4)return 2;
//	outw=width/4;
//	outh=height/4;
//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XE0,0X04);
//	SCCB_WR_Reg(0X5A,outw&0XFF);		//����OUTW�ĵͰ�λ
//	SCCB_WR_Reg(0X5B,outh&0XFF);		//����OUTH�ĵͰ�λ
//	temp=(outw>>8)&0X03;
//	temp|=(outh>>6)&0X04;
//	SCCB_WR_Reg(0X5C,temp);				//����OUTH/OUTW�ĸ�λ
//	SCCB_WR_Reg(0XE0,0X00);
//	return 0;
//}
////����ͼ�񿪴���С
////��:OV2640_ImageSize_Setȷ������������ֱ��ʴӴ�С.
////�ú������������Χ������п���,����OV2640_OutSize_Set�����
////ע��:�������Ŀ�Ⱥ͸߶�,������ڵ���OV2640_OutSize_Set�����Ŀ�Ⱥ͸߶�
////     OV2640_OutSize_Set���õĿ�Ⱥ͸߶�,���ݱ��������õĿ�Ⱥ͸߶�,��DSP
////     �Զ��������ű���,������ⲿ�豸.
////width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
////����ֵ:0,���óɹ�
////    ����,����ʧ��
//u8 OV2640_ImageWin_Set(u16 offx,u16 offy,u16 width,u16 height)
//{
//	u16 hsize;
//	u16 vsize;
//	u8 temp;
//	if(width%4)return 1;
//	if(height%4)return 2;
//	hsize=width/4;
//	vsize=height/4;
//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XE0,0X04);
//	SCCB_WR_Reg(0X51,hsize&0XFF);		//����H_SIZE�ĵͰ�λ
//	SCCB_WR_Reg(0X52,vsize&0XFF);		//����V_SIZE�ĵͰ�λ
//	SCCB_WR_Reg(0X53,offx&0XFF);		//����offx�ĵͰ�λ
//	SCCB_WR_Reg(0X54,offy&0XFF);		//����offy�ĵͰ�λ
//	temp=(vsize>>1)&0X80;
//	temp|=(offy>>4)&0X70;
//	temp|=(hsize>>5)&0X08;
//	temp|=(offx>>8)&0X07;
//	SCCB_WR_Reg(0X55,temp);				//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
//	SCCB_WR_Reg(0X57,(hsize>>2)&0X80);	//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
//	SCCB_WR_Reg(0XE0,0X00);
//	return 0;
//}
////�ú�������ͼ��ߴ��С,Ҳ������ѡ��ʽ������ֱ���
////UXGA:1600*1200,SVGA:800*600,CIF:352*288
////width,height:ͼ���Ⱥ�ͼ��߶�
////����ֵ:0,���óɹ�
////    ����,����ʧ��
//u8 OV2640_ImageSize_Set(u16 width,u16 height)
//{
//	u8 temp;
//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XE0,0X04);
//	SCCB_WR_Reg(0XC0,(width)>>3&0XFF);		//����HSIZE��10:3λ
//	SCCB_WR_Reg(0XC1,(height)>>3&0XFF);		//����VSIZE��10:3λ
//	temp=(width&0X07)<<3;
//	temp|=height&0X07;
//	temp|=(width>>4)&0X80;
//	SCCB_WR_Reg(0X8C,temp);
//	SCCB_WR_Reg(0XE0,0X00);
//	return 0;
//}

//OV5640д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 OV5640_WR_Reg(u16 reg, u8 data)
{
    u8 res = 0;
    SCCB_Start(); 					//����SCCB����
    if(SCCB_WR_Byte(OV5640_ADDR))res = 1;	//д����ID
    if(SCCB_WR_Byte(reg >> 8))res = 1;	//д�Ĵ�����8λ��ַ
    if(SCCB_WR_Byte(reg))res = 1;		//д�Ĵ�����8λ��ַ
    if(SCCB_WR_Byte(data))res = 1; 	//д����
    SCCB_Stop();
    return	res;
}
//OV5640���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 OV5640_RD_Reg(u16 reg)
{
    u8 val = 0;
    SCCB_Start(); 				//����SCCB����
    SCCB_WR_Byte(OV5640_ADDR);	//д����ID
    SCCB_WR_Byte(reg >> 8);	  //д�Ĵ�����8λ��ַ
    SCCB_WR_Byte(reg);			//д�Ĵ�����8λ��ַ
    SCCB_Stop();
    //���üĴ�����ַ�󣬲��Ƕ�
    SCCB_Start();
    SCCB_WR_Byte(OV5640_ADDR | 0X01); //���Ͷ�����
    val = SCCB_RD_Byte();		 	//��ȡ����
    SCCB_No_Ack();
    SCCB_Stop();
    return val;
}
//��ʼ��OV5640

//����ֵ:0,�ɹ�
//    ����,�������
u8 OV5640_Init(void)
{   p_dbg_enter;
    u16 i = 0;
    u16 reg;
    //����IO
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOD,Eʱ��
    //OV_RESET OV_PWDN ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//25MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//GPIOD6�������
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//GPIOE5�������
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��

    OV5640_RST = 0;			//����������OV5640��RST��,���ϵ�
    delay_ms(20);
    OV5640_PWDN = 0;		//POWER ON
    delay_ms(5);
    OV5640_RST = 1;			//������λ
    delay_ms(20);
    SCCB_Init();			//��ʼ��SCCB ��IO��
    delay_ms(5);
    reg = OV5640_RD_Reg(OV5640_CHIPIDH);	//��ȡID �߰�λ
    reg <<= 8;
    reg |= OV5640_RD_Reg(OV5640_CHIPIDL);	//��ȡID �Ͱ�λ
    if(reg != OV5640_ID)
    {
        printf("ID:%d\r\n", reg);
        return 1;
    }
    OV5640_WR_Reg(0x3103, 0X11);	//system clock from pad, bit[1]
    OV5640_WR_Reg(0X3008, 0X82);	//��λ
    delay_ms(10);
    //��ʼ�� OV5640
    for(i = 0; i < sizeof(ov5640_init_reg_tbl) / 4; i++)
    {
        OV5640_WR_Reg(ov5640_init_reg_tbl[i][0], ov5640_init_reg_tbl[i][1]);
    }
    //���������Ƿ�����
//	OV5640_Flash_Ctrl(1);//�������
//	delay_ms(50);
//	OV5640_Flash_Ctrl(0);//�ر������
    p_dbg_exit;
    return 0x00; 	//ok
}
//OV5640�л�ΪJPEGģʽ
void OV5640_JPEG_Mode(void)
{
    u16 i = 0;
    //����:���JPEG����
    for(i = 0; i < (sizeof(OV5640_jpeg_reg_tbl) / 4); i++)
    {
        OV5640_WR_Reg(OV5640_jpeg_reg_tbl[i][0], OV5640_jpeg_reg_tbl[i][1]);
    }
}
//OV5640�л�ΪRGB565ģʽ
void OV5640_RGB565_Mode(void)
{   p_dbg_enter;
    u16 i = 0;
    //����:RGB565���
    for(i = 0; i < (sizeof(ov5640_rgb565_reg_tbl) / 4); i++)
    {
        OV5640_WR_Reg(ov5640_rgb565_reg_tbl[i][0], ov5640_rgb565_reg_tbl[i][1]);
    }
    p_dbg_exit ;
}
//EV�عⲹ�����ò�����֧��7���ȼ�
const static u8 OV5640_EXPOSURE_TBL[7][6] =
{
    0x10, 0x08, 0x10, 0x08, 0x20, 0x10, //-3
    0x20, 0x18, 0x41, 0x20, 0x18, 0x10, //-
    0x30, 0x28, 0x61, 0x30, 0x28, 0x10, //-1
    0x38, 0x30, 0x61, 0x38, 0x30, 0x10, //0
    0x40, 0x38, 0x71, 0x40, 0x38, 0x10, //+1
    0x50, 0x48, 0x90, 0x50, 0x48, 0x20, //+2
    0x60, 0x58, 0xa0, 0x60, 0x58, 0x20, //+3
};

//EV�عⲹ��
//exposure:0~6,������-3~3.
void OV5640_Exposure(u8 exposure)
{
    OV5640_WR_Reg(0x3212, 0x03);	//start group 3
    OV5640_WR_Reg(0x3a0f, OV5640_EXPOSURE_TBL[exposure][0]);
    OV5640_WR_Reg(0x3a10, OV5640_EXPOSURE_TBL[exposure][1]);
    OV5640_WR_Reg(0x3a1b, OV5640_EXPOSURE_TBL[exposure][2]);
    OV5640_WR_Reg(0x3a1e, OV5640_EXPOSURE_TBL[exposure][3]);
    OV5640_WR_Reg(0x3a11, OV5640_EXPOSURE_TBL[exposure][4]);
    OV5640_WR_Reg(0x3a1f, OV5640_EXPOSURE_TBL[exposure][5]);
    OV5640_WR_Reg(0x3212, 0x13); //end group 3
    OV5640_WR_Reg(0x3212, 0xa3); //launch group 3
}

//�ƹ�ģʽ������,֧��5��ģʽ
const static u8 OV5640_LIGHTMODE_TBL[5][7] =
{
    0x04, 0X00, 0X04, 0X00, 0X04, 0X00, 0X00, //Auto,�Զ�
    0x06, 0X1C, 0X04, 0X00, 0X04, 0XF3, 0X01, //Sunny,�չ�
    0x05, 0X48, 0X04, 0X00, 0X07, 0XCF, 0X01, //Office,�칫��
    0x06, 0X48, 0X04, 0X00, 0X04, 0XD3, 0X01, //Cloudy,����
    0x04, 0X10, 0X04, 0X00, 0X08, 0X40, 0X01, //Home,����
};
//��ƽ������
//0:�Զ�
//1:�չ�sunny
//2,�칫��office
//3,����cloudy
//4,����home
void OV5640_Light_Mode(u8 mode)
{   p_dbg_enter;
    u8 i;
    OV5640_WR_Reg(0x3212, 0x03);	//start group 3
    for(i = 0; i < 7; i++)OV5640_WR_Reg(0x3400 + i, OV5640_LIGHTMODE_TBL[mode][i]); //���ñ��Ͷ�
    OV5640_WR_Reg(0x3212, 0x13); //end group 3
    OV5640_WR_Reg(0x3212, 0xa3); //launch group 3
    p_dbg_exit ;
}
//ɫ�ʱ��Ͷ����ò�����,֧��7���ȼ�
const static u8 OV5640_SATURATION_TBL[7][6] =
{
    0X0C, 0x30, 0X3D, 0X3E, 0X3D, 0X01, //-3
    0X10, 0x3D, 0X4D, 0X4E, 0X4D, 0X01, //-2
    0X15, 0x52, 0X66, 0X68, 0X66, 0X02, //-1
    0X1A, 0x66, 0X80, 0X82, 0X80, 0X02, //+0
    0X1F, 0x7A, 0X9A, 0X9C, 0X9A, 0X02, //+1
    0X24, 0x8F, 0XB3, 0XB6, 0XB3, 0X03, //+2
    0X2B, 0xAB, 0XD6, 0XDA, 0XD6, 0X04, //+3
};
//ɫ������
//sat:0~6,�����Ͷ�-3~3.
void OV5640_Color_Saturation(u8 sat)
{   p_dbg_enter;
    u8 i;
    OV5640_WR_Reg(0x3212, 0x03);	//start group 3
    OV5640_WR_Reg(0x5381, 0x1c);
    OV5640_WR_Reg(0x5382, 0x5a);
    OV5640_WR_Reg(0x5383, 0x06);
    for(i = 0; i < 6; i++)OV5640_WR_Reg(0x5384 + i, OV5640_SATURATION_TBL[sat][i]); //���ñ��Ͷ�
    OV5640_WR_Reg(0x538b, 0x98);
    OV5640_WR_Reg(0x538a, 0x01);
    OV5640_WR_Reg(0x3212, 0x13); //end group 3
    OV5640_WR_Reg(0x3212, 0xa3); //launch group 3
    p_dbg_exit;
}
//��������
//bright:0~8,��������-4~4.
void OV5640_Brightness(u8 bright)
{   p_dbg_enter;
    u8 brtval;
    if(bright < 4)brtval = 4 - bright;
    else brtval = bright - 4;
    OV5640_WR_Reg(0x3212, 0x03);	//start group 3
    OV5640_WR_Reg(0x5587, brtval << 4);
    if(bright < 4)OV5640_WR_Reg(0x5588, 0x09);
    else OV5640_WR_Reg(0x5588, 0x01);
    OV5640_WR_Reg(0x3212, 0x13); //end group 3
    OV5640_WR_Reg(0x3212, 0xa3); //launch group 3
    p_dbg_exit;
}
//�Աȶ�����
//contrast:0~6,��������-3~3.
void OV5640_Contrast(u8 contrast)
{   p_dbg_enter;
    u8 reg0val = 0X00; //contrast=3,Ĭ�϶Աȶ�
    u8 reg1val = 0X20;
    switch(contrast)
    {
    case 0://-3
        reg1val = reg0val = 0X14;
        break;
    case 1://-2
        reg1val = reg0val = 0X18;
        break;
    case 2://-1
        reg1val = reg0val = 0X1C;
        break;
    case 4://1
        reg0val = 0X10;
        reg1val = 0X24;
        break;
    case 5://2
        reg0val = 0X18;
        reg1val = 0X28;
        break;
    case 6://3
        reg0val = 0X1C;
        reg1val = 0X2C;
        break;
    }
    OV5640_WR_Reg(0x3212, 0x03); //start group 3
    OV5640_WR_Reg(0x5585, reg0val);
    OV5640_WR_Reg(0x5586, reg1val);
    OV5640_WR_Reg(0x3212, 0x13); //end group 3
    OV5640_WR_Reg(0x3212, 0xa3); //launch group 3
    p_dbg_exit;
}
//�������
//sharp:0~33,0,�ر�;33,auto;����ֵ,��ȷ�Χ.
void OV5640_Sharpness(u8 sharp)
{   p_dbg_enter;
    if(sharp < 33) //�������ֵ
    {
        OV5640_WR_Reg(0x5308, 0x65);
        OV5640_WR_Reg(0x5302, sharp);
    } else	//�Զ����
    {
        OV5640_WR_Reg(0x5308, 0x25);
        OV5640_WR_Reg(0x5300, 0x08);
        OV5640_WR_Reg(0x5301, 0x30);
        OV5640_WR_Reg(0x5302, 0x10);
        OV5640_WR_Reg(0x5303, 0x00);
        OV5640_WR_Reg(0x5309, 0x08);
        OV5640_WR_Reg(0x530a, 0x30);
        OV5640_WR_Reg(0x530b, 0x04);
        OV5640_WR_Reg(0x530c, 0x06);
    }
    p_dbg_exit;

}
//��Ч���ò�����,֧��7����Ч
const static u8 OV5640_EFFECTS_TBL[7][3] =
{
    0X06, 0x40, 0X10, //����
    0X1E, 0xA0, 0X40, //��ɫ
    0X1E, 0x80, 0XC0, //ůɫ
    0X1E, 0x80, 0X80, //�ڰ�
    0X1E, 0x40, 0XA0, //����
    0X40, 0x40, 0X10, //��ɫ
    0X1E, 0x60, 0X60, //ƫ��
};
//��Ч����
//0:����
//1,��ɫ
//2,ůɫ
//3,�ڰ�
//4,ƫ��
//5,��ɫ
//6,ƫ��
void OV5640_Special_Effects(u8 eft)
{
    OV5640_WR_Reg(0x3212, 0x03); //start group 3
    OV5640_WR_Reg(0x5580, OV5640_EFFECTS_TBL[eft][0]);
    OV5640_WR_Reg(0x5583, OV5640_EFFECTS_TBL[eft][1]); // sat U
    OV5640_WR_Reg(0x5584, OV5640_EFFECTS_TBL[eft][2]); // sat V
    OV5640_WR_Reg(0x5003, 0x08);
    OV5640_WR_Reg(0x3212, 0x13); //end group 3
    OV5640_WR_Reg(0x3212, 0xa3); //launch group 3
}
//��������
//mode:0,�ر�
//     1,����
//     2,ɫ��
void OV5640_Test_Pattern(u8 mode)
{
    if(mode == 0)OV5640_WR_Reg(0X503D, 0X00);
    else if(mode == 1)OV5640_WR_Reg(0X503D, 0X80);
    else if(mode == 2)OV5640_WR_Reg(0X503D, 0X82);
}
//����ƿ���
//mode:0,�ر�
//     1,��
void OV5640_Flash_Ctrl(u8 sw)
{
    OV5640_WR_Reg(0x3016, 0X02);
    OV5640_WR_Reg(0x301C, 0X02);
    if(sw)OV5640_WR_Reg(0X3019, 0X02);
    else OV5640_WR_Reg(0X3019, 0X00);
}
//����ͼ�������С
//OV5640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸú���ȷ��
//offx,offy,Ϊ���ͼ����OV5640_ImageWin_Set�趨����(���賤��Ϊxsize��ysize)�ϵ�ƫ��
//���ڿ�����scale����,���������ͼ�񴰿�Ϊ:xsize-2*offx,ysize-2*offy
//width,height:ʵ�����ͼ��Ŀ�Ⱥ͸߶�
//ʵ�����(width,height),����xsize-2*offx,ysize-2*offy�Ļ����Ͻ������Ŵ���.
//һ������offx��offy��ֵΪ16��4,��СҲ�ǿ���,����Ĭ����16��4
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 OV5640_OutSize_Set(u16 offx, u16 offy, u16 width, u16 height)
{
    OV5640_WR_Reg(0X3212, 0X03);  	//��ʼ��3
    //�������þ���ʵ������ߴ�(������)
    OV5640_WR_Reg(0x3808, width >> 8);	//����ʵ�������ȸ��ֽ�
    OV5640_WR_Reg(0x3809, width & 0xff); //����ʵ�������ȵ��ֽ�
    OV5640_WR_Reg(0x380a, height >> 8); //����ʵ������߶ȸ��ֽ�
    OV5640_WR_Reg(0x380b, height & 0xff); //����ʵ������߶ȵ��ֽ�
    //�������þ�������ߴ���ISP�����ȡͼ��Χ
    //��Χ:xsize-2*offx,ysize-2*offy
    OV5640_WR_Reg(0x3810, offx >> 8);	//����X offset���ֽ�
    OV5640_WR_Reg(0x3811, offx & 0xff); //����X offset���ֽ�

    OV5640_WR_Reg(0x3812, offy >> 8);	//����Y offset���ֽ�
    OV5640_WR_Reg(0x3813, offy & 0xff); //����Y offset���ֽ�

    OV5640_WR_Reg(0X3212, 0X13);		//������3
    OV5640_WR_Reg(0X3212, 0Xa3);		//������3����
    return 0;
}

//����ͼ�񿪴���С(ISP��С),�Ǳ�Ҫ,һ��������ô˺���
//���������������濪��(���2592*1944),����OV5640_OutSize_Set�����
//ע��:�������Ŀ�Ⱥ͸߶�,������ڵ���OV5640_OutSize_Set�����Ŀ�Ⱥ͸߶�
//     OV5640_OutSize_Set���õĿ�Ⱥ͸߶�,���ݱ��������õĿ�Ⱥ͸߶�,��DSP
//     �Զ��������ű���,������ⲿ�豸.
//width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 OV5640_ImageWin_Set(u16 offx, u16 offy, u16 width, u16 height)
{
    u16 xst, yst, xend, yend;
    xst = offx;
    yst = offy;
    xend = offx + width - 1;
    yend = offy + height - 1;
    OV5640_WR_Reg(0X3212, 0X03);		//��ʼ��3
    OV5640_WR_Reg(0X3800, xst >> 8);
    OV5640_WR_Reg(0X3801, xst & 0XFF);
    OV5640_WR_Reg(0X3802, yst >> 8);
    OV5640_WR_Reg(0X3803, yst & 0XFF);
    OV5640_WR_Reg(0X3804, xend >> 8);
    OV5640_WR_Reg(0X3805, xend & 0XFF);
    OV5640_WR_Reg(0X3806, yend >> 8);
    OV5640_WR_Reg(0X3807, yend & 0XFF);
    OV5640_WR_Reg(0X3212, 0X13);		//������3
    OV5640_WR_Reg(0X3212, 0Xa3);		//������3����
    return 0;
}
//��ʼ���Զ��Խ�
//����ֵ:0,�ɹ�;1,ʧ��.
u8 OV5640_Focus_Init(void)
{   p_dbg_enter;
    u16 i;
    u16 addr = 0x8000;
    u8 state = 0x8F;
    OV5640_WR_Reg(0x3000, 0x20);			//reset MCU
    for(i = 0; i < sizeof(OV5640_AF_Config); i++) //������������
    {
        OV5640_WR_Reg(addr, OV5640_AF_Config[i]);
        addr++;
    }
    OV5640_WR_Reg(0x3022, 0x00);
    OV5640_WR_Reg(0x3023, 0x00);
    OV5640_WR_Reg(0x3024, 0x00);
    OV5640_WR_Reg(0x3025, 0x00);
    OV5640_WR_Reg(0x3026, 0x00);
    OV5640_WR_Reg(0x3027, 0x00);
    OV5640_WR_Reg(0x3028, 0x00);
    OV5640_WR_Reg(0x3029, 0x7f);
    OV5640_WR_Reg(0x3000, 0x00);
    i = 0;
    do
    {
        state = OV5640_RD_Reg(0x3029);
        delay_ms(5);
        i++;
        if(i > 1000)return 1;
    } while(state != 0x70);
    p_dbg_exit ;
    return 0;
}
//ִ��һ���Զ��Խ�
//����ֵ:0,�ɹ�;1,ʧ��.
u8 OV5640_Focus_Single(void)
{   p_dbg_enter;
    u8 temp;
    u16 retry = 0;
    OV5640_WR_Reg(0x3022, 0x03);		//����һ���Զ��Խ�
    while(1)
    {
        retry++;
        temp = OV5640_RD_Reg(0x3029);	//���Խ����״̬
        if(temp == 0x10)break;		// focus completed
        delay_ms(5);
        if(retry > 1000)return 1;
    }
    p_dbg_exit;
    return 0;
}
//�����Զ��Խ�,��ʧ����,���Զ������Խ�
//����ֵ:0,�ɹ�;����,ʧ��.
u8 OV5640_Focus_Constant(void)
{
    u8 temp = 0;
    u16 retry = 0;
    OV5640_WR_Reg(0x3023, 0x01);
    OV5640_WR_Reg(0x3022, 0x08); //����IDLEָ��
    do
    {
        temp = OV5640_RD_Reg(0x3023);
        retry++;
        if(retry > 1000)return 2;
        delay_ms(5);
    } while(temp != 0x00);
    OV5640_WR_Reg(0x3023, 0x01);
    OV5640_WR_Reg(0x3022, 0x04); //���ͳ����Խ�ָ��
    retry = 0;
    do
    {
        temp = OV5640_RD_Reg(0x3023);
        retry++;
        if(retry > 1000)return 2;
        delay_ms(5);
    } while(temp != 0x00); //0,�Խ����;1:���ڶԽ�
    return 0;
}

