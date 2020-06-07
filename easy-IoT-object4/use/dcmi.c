#define DEBUG
#include "stdarg.h"
#include "stdio.h"

#include "drivers.h"
#include "bsp.h"
#include "debug.h"
//#include "ov2640.h" 


//extern void jpeg_data_process(void);	//JPEG���ݴ�����

//DCMI_InitTypeDef DCMI_InitStructure;


//DCMI DMA����
//DMA_Memory0BaseAddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��  
//DMA_MemoryDataSize:�洢��λ��    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:�洢��������ʽ  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable
//void DCMI_DMA_Init(uint32_t *DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 mode)
//{ 
//	DMA_InitTypeDef  DMA_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
//	DMA_DeInit(DMA2_Stream1);
//	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA2_Stream1������ 
//	
//  /* ���� DMA Stream */
//  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //ͨ��1 DCMIͨ�� 
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DCMI->DR;//�����ַΪ:DCMI->DR
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DMA_Memory0BaseAddr;//DMA �洢��0��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
//  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;//���ݴ����� 
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
//  DMA_InitStructure.DMA_MemoryInc = mode;//�洢������ģʽ
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//�������ݳ���:32λ
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;//�洢�����ݳ��� 
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFOģʽ        
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//ʹ��ȫFIFO 
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
//  DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream
//	
//} 
////DCMI��ʼ��
//void My_DCMI_Init(void)
//{
//	
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	DCMI_InitTypeDef DCMI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//ʹ��DCMIʱ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|
//												 RCC_AHB1Periph_GPIOB|
//												 RCC_AHB1Periph_GPIOC|
//	                       RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA B C E ʱ��
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù������
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//	//GPIOA4,6
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIOA��ʼ��
//	//GPIOB6,7,8
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIOB��ʼ��
//	//GPIOC6,7
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//GPIOC��ʼ��
//	//GPIOE0,1,4,6
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_6;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIOE��ʼ��	
//	
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
// 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
// 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PC6,AF13  DCMI_D0  
// 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI);	//PC7,AF13 	DCMI_D1 
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_DCMI); //PE0,AF13  DCMI_D2
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_DCMI); //PE1,AF13  DCMI_D3
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource4,GPIO_AF_DCMI); //PE4,AF13  DCMI_D4 
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_DCMI); //PB6,AF13  DCMI_D5 
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_DCMI); //PB8,AF13  DCMI_D6
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_DCMI); //PE6,AF13  DCMI_D7

//	DCMI_DeInit();//���ԭ�������� 
// 
//    DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
//	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
//	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
//	DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC �͵�ƽ��Ч
//	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK ��������Ч
//	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
//	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_Low;//VSYNC �͵�ƽ��Ч
//	DCMI_Init(&DCMI_InitStructure);
//	DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//����֡�ж� 
//	
//	DCMI_Cmd(ENABLE);	//DCMIʹ��

//    NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=DCMI_IRQn_Priority;//��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
// 
//} 



////DCMI,��������
//void DCMI_Start(void)
//{  
//    LCD_SetCursor(0,0);  
//	LCD_WriteRAM_Prepare();		       //��ʼд��GRAM
//	DMA_Cmd(DMA2_Stream1, ENABLE);//����DMA2,Stream1 
//	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ��  
//}
////DCMI,�رմ���
//void DCMI_Stop(void)
//{ 
//  DCMI_CaptureCmd(DISABLE);//DCMI����ʹ�ر�	
//	
//	while(DCMI->CR&0X01);		//�ȴ�������� 
//	 	
//	DMA_Cmd(DMA2_Stream1,DISABLE);//�ر�DMA2,Stream1
//} 

u8 ov_frame=0;  						//֡��
extern void jpeg_data_process(void);	//JPEG���ݴ�����

DCMI_InitTypeDef DCMI_InitStructure;

//DCMI�жϷ�����
//void DCMI_IRQHandler(void)
//{
//	if(DCMI_GetITStatus(DCMI_IT_FRAME)==SET)//����һ֡ͼ��
//	{
//		jpeg_data_process(); 	//jpeg���ݴ���	
//		DCMI_ClearITPendingBit(DCMI_IT_FRAME);//���֡�ж�
////		LED1=!LED1;
//		ov_frame++;  
//	}
//} 
//DCMI DMA����
//DMA_Memory0BaseAddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��  
//DMA_MemoryDataSize:�洢��λ��    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:�洢��������ʽ  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable

void DCMI_DMA_Init(uint32_t *DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 mode)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA2_Stream1������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //ͨ��1 DCMIͨ�� 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DCMI->DR;//�����ַΪ:DCMI->DR
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DMA_Memory0BaseAddr;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = mode;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//�������ݳ���:32λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;//�洢�����ݳ��� 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFOģʽ        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//ʹ��ȫFIFO 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream
	
} 
//DCMI��ʼ��
void My_DCMI_Init(void)
{
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	DCMI_InitTypeDef DCMI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//ʹ��DCMIʱ��
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA|
							RCC_AHB1Periph_GPIOB|
							RCC_AHB1Periph_GPIOC|
							RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA B C E ʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	//GPIOA4,6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIOA��ʼ��
	//GPIOB6,7,8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIOB��ʼ��
	//GPIOC6,7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//GPIOC��ʼ��
	//GPIOE0,1,4,6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIOE��ʼ��	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PC6,AF13  DCMI_D0  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI);	//PC7,AF13 	DCMI_D1 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_DCMI); //PE0,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_DCMI); //PE1,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource4,GPIO_AF_DCMI); //PE4,AF13  DCMI_D4 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_DCMI); //PB6,AF13  DCMI_D5 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_DCMI); //PB8,AF13  DCMI_D6
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_DCMI); //PE6,AF13  DCMI_D7

	DCMI_DeInit();//���ԭ�������� 
 
    DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
	DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_Low;//VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);
	DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//����֡�ж� 
	
	DCMI_Cmd(ENABLE);	//DCMIʹ��

    NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=DCMI_IRQn_Priority;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
 
} 
//DCMI,��������
void DCMI_Start(void)
{  
    LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();		//��ʼд��GRAM
	DMA_Cmd(DMA2_Stream1, ENABLE);//����DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ��  
}
//DCMI,�رմ���
void DCMI_Stop(void)
{ 
  DCMI_CaptureCmd(DISABLE);//DCMI����ʹ�ر�	
	
	while(DCMI->CR&0X01);		//�ȴ�������� 
	 	
	DMA_Cmd(DMA2_Stream1,DISABLE);//�ر�DMA2,Stream1
} 
////////////////////////////////////////////////////////////////////////////////
//������������,��usmart����,���ڵ��Դ���

//DCMI������ʾ����
//sx,sy;LCD����ʼ����
//width,height:LCD��ʾ��Χ.
void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{
	DCMI_Stop(); 
	LCD_Clear(WHITE);
	LCD_Set_Window(sx,sy,width,height);
	OV5640_OutSize_Set(4,0,width,height);
  LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();		//��ʼд��GRAM
	
	DMA_Cmd(DMA2_Stream1,ENABLE);	//����DMA2,Stream1 
	
	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ�� 
	
}
   
//ͨ��usmart����,����������.
//pclk/hsync/vsync:�����źŵ����޵�ƽ����
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync)
{
	DCMI_DeInit();//���ԭ�������� 
 
  DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
	DCMI_InitStructure.DCMI_HSPolarity= hsync<<6;//HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity= pclk<<5;//PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=vsync<<7;//VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);

	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ�� 
	DCMI_Cmd(ENABLE);	//DCMIʹ��
  
}



