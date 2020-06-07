#define DEBUG
#include "drivers.h"
#include "bsp.h"

#include "debug.h"

//ʹ��TIM4��PWM������ƵƵ����ȣ�������D13���Ͻ�һ��USBС̨�ƣ��۲�̨�����ȵı仯��̨��������D13�������ӵأ�

uint32_t Period_tot = 8000;

void lamp_pwm_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	uint32_t apbclock, scaler = 0, Counter;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	TIM_DeInit(TIM4); 
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	Counter = apbclock*2/(scaler + 1)/38000;
	

	GPIO_PinAFConfig(IO4_GROUP, GPIO_PinSource13, GPIO_AF_TIM4); 
	gpio_cfg((uint32_t)IO4_GROUP, IO4_PIN, GPIO_Mode_AF_PP);
	
	TIM_TimeBaseStructure.TIM_Period = Period_tot;          //һ����������ΪPeriod_tot��ʱ��
 	//TIM_TimeBaseStructure.TIM_Prescaler = 84000-1;
 	TIM_TimeBaseStructure.TIM_Prescaler = 84 -1;           //ʱ��Ԥ��Ƶ����ֵԽС���������������ԽС
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Period_tot/2;          //��������ķ���ռ�ձ�Ϊ1/2������̨�Ƴ�ʼ����Ϊ50
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_Low;        //���û������������
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ��������
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //���������״̬
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//�����󻥲������״̬
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//����ģʽ�����ѡ�� 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//����ģʽ�����ѡ�� 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //��������
    TIM_BDTRInitStructure.TIM_DeadTime = 0x90;                                         //����ʱ������
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 //ɲ������ʹ��
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;//ɲ�����뼫��
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ�� 
    //TIM_BDTRConfig(TIM4,&TIM_BDTRInitStructure);

    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
	TIM_CtrlPWMOutputs(TIM4, ENABLE);                         //��ʼPWM���

}

//�ı�PWMռ�ձȣ�ʵ��̨�����ȵ�����������Χ0~100.
void lamp_pwm_set(int stat)
{
    if(stat > 100 || stat < 0) return;
	if(stat == 0) stat = 1;
	if(stat == 100) stat = 99;
	
	TIM_SetCompare2(TIM4, Period_tot*stat/100);
}


