#define DEBUG
#include "stdarg.h"
#include "stdio.h"

#include "drivers.h"
#include "bsp.h"
#include "debug.h"

//���

#define SlaveAddress   0x46											   //BH1750FVIģ��ADDR�ӵͣ���ѰַΪ0110011
//extern GPIO_InitTypeDef GPIO_InitStructure;
#define SCL_H    GPIO_SetBits(GPIOD,GPIO_Pin_10);					   
#define SCL_L    GPIO_ResetBits(GPIOD,GPIO_Pin_10);
#define SDA_H    GPIO_SetBits(GPIOD,GPIO_Pin_12);
#define SDA_L    GPIO_ResetBits(GPIOD,GPIO_Pin_12);

extern void delay_us(uint32_t count);
extern void delay_ms(uint32_t us);
void BH1750_Start(void);	                                           //IIC��ʼ����
void BH1750_SendByte(u8);											   //IIC�����ֽں���
u8   BH1750_ReadSlaveAck(void);										   //��ȡSDA��Ӧ�ź�
void BH1750_WriteCommand(u8);										   //д�����������SendByte����
float BH1750_ReadResult(void);										   //��ȡת����������������
u8   BH1750_ReadByte(void);											   //��ȡת��������ֽ����
void BH1750_SendACK(u8);											   //��������Ӧ��ACK����Ӧ��
void BH1750_Stop(void);												   //IICֹͣ����
void IIC_InputConfig(void);											   //I/O�л�Ϊ��������
void IIC_OutputConfig(void);

uint32_t BH1750FVI(void)
{
  float  light=0;										//����ǿ��
  uint32_t ret;
  
  BH1750_WriteCommand(0x01);						//�ϵ��ʼ��
  delay_ms(100);

 
  BH1750_WriteCommand(0x01);                      //ͨ��
  BH1750_WriteCommand(0x10);                      //�����߷ֱ���ģʽ
  delay_ms(100);                                 //��ʱ���

  light= (BH1750_ReadResult());                     //�����������ݣ��洢��BUF�� 

  //p_dbg("light strength raw[%f]", light);
  ret = (uint32_t)light;
  return(ret);
}


/**************************************
���ܣ�������ʼ�ź�
**************************************/
void BH1750_Start()
{
    IIC_OutputConfig();
    SDA_H;                                          //����������
    SCL_H;                                          //����ʱ����
    delay_us(5);                                    //��ʱ
    SDA_L;                                          //�����½���
    delay_us(5);                                    //��ʱ
    SCL_L;                                          //����ʱ����
}


/**************************************
���ܣ���IIC���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(u8 dat)
{
    u8 i;
    IIC_OutputConfig();

    for (i=0; i<8; i++)                             //8λ������
    {
        if(dat&0x80){ SDA_H; }    
        else        { SDA_L; }
          
        dat <<= 1;                                  //�Ƴ����ݵ����λ
        SCL_H;                                      //����ʱ����
        delay_us(5);                                //��ʱ
        SCL_L;                                      //����ʱ����
        delay_us(5);                                //��ʱ
    }

    BH1750_ReadSlaveAck();

}


/**************************************
���ܣ���ȡSDAӦ���ź�
**************************************/
u8 BH1750_ReadSlaveAck()       
{
    u8 ACK;
    IIC_InputConfig();

    SCL_H;                                           //����ʱ����
    delay_us(5);                                     //��ʱ
    ACK=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12);    //��Ӧ���ź�

    SCL_L;                                           //����ʱ����
    delay_us(5);                                     //��ʱ

    return ACK;
}

/****************************************
���ܣ�����һ���ֽڵ����ݣ�����SendByte����
*****************************8888888****/
void BH1750_WriteCommand(u8 REG_Address)
{
    BH1750_Start();                                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);                   //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);                    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    BH1750_Stop();                                   //����ֹͣ�ź�
}

/*********************************
���ܣ���ȡģ��ת�����ݣ����������
*********************************/
float BH1750_ReadResult()
{
    u8  resultH=0,resultL=0;
    float result=0;

    BH1750_Start();                                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);                 //�����豸��ַ+д�ź�

    resultH=BH1750_ReadByte();
    BH1750_SendACK(0);
    resultL=BH1750_ReadByte();
    BH1750_SendACK(1);
    //p_dbg("light resultH[%x]resultL[%x]", resultH, resultL);
    
    //result=(resultH*pow(2,8)+resultL)/1.2;
    result = (float)(((int)resultH << 8) + resultL)/1.2;

    BH1750_Stop();                                   //����ֹͣ�ź�

    return result;

}
/**************************************
���ܣ���IIC���߽���һ���ֽ�����
**************************************/
u8 BH1750_ReadByte()
{
    u8 i;
    u8 dat = 0;
    IIC_OutputConfig();

    SDA_H;                                                  //ʹ���ڲ�����,׼����ȡ����,
    delay_us(2);

    IIC_InputConfig();
    for (i=0; i<8; i++)                                     //8λ������
    {
        dat <<= 1;           
        SCL_H;                                              //����ʱ����
        delay_us(5);                                        //��ʱ
        dat |= GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12);    //������               
        SCL_L;                                              //����ʱ����
        delay_us(5);                                        //��ʱ
    }
    return dat;
}


/**************************************
���ܣ�����Ӧ���ź�
����: ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(u8 ack)
{
    IIC_OutputConfig();

    if(ack==1) {SDA_H; }
    if(ack==0) {SDA_L; }
    SCL_H;                                       //����ʱ����
    delay_us(5);                                 //��ʱ
    SCL_L;                                       //����ʱ����
    delay_us(5);                                 //��ʱ
}


/**************************************
����:IIC����ֹͣ�ź�
**************************************/
void BH1750_Stop()
{
    IIC_OutputConfig();
    SDA_L;                                       //����������
    SCL_H;                                       //����ʱ����
    delay_us(5);                                 //��ʱ
    SDA_H;                                       //����������
    delay_us(5);                                 //��ʱ
}




/***************************************
���ܣ�SDA����Ϊ��������
****************************************/
void IIC_InputConfig()
{
    gpio_cfg((uint32_t)GPIOD, GPIO_Pin_12, GPIO_Mode_IN_FLOATING);
}

/***************************************
���ܣ�IIC����Ϊ�������
****************************************/
void IIC_OutputConfig()
{
    gpio_cfg((uint32_t)GPIOD, GPIO_Pin_12, GPIO_Mode_Out_PP);
}

void BH1750_I2C_Init(void)
{
    //GPIO_InitTypeDef  GPIO_InitStructure; 
    /* Configure I2C1 pins: SCL and SDA */
    gpio_cfg((uint32_t)GPIOD, GPIO_Pin_10, GPIO_Mode_Out_PP);
    gpio_cfg((uint32_t)GPIOD, GPIO_Pin_12, GPIO_Mode_Out_PP);
}
uint32_t get_BH1750_value(void)
{
    return BH1750FVI();
}

