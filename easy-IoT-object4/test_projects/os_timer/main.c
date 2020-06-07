#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "test.h"
#include "dhcpd.h"
#include "web_cfg.h"
#include <cctype>


#define VIRSION		"T1.0"

char command;
int dbg_level = 1;

/*
*�պ���,����������
*/
void send_work_event(uint32_t event)
{

}

/*
*�պ���,����������
*/
void show_tcpip_info(struct netif *p_netif)
{
}

timer_t timer1, timer2;

void test_timer_cb1(void *pdata)
{
	p_dbg("��ʱ��1�ص�");
	
}

void test_timer_cb2(void *pdata)
{
	p_dbg("��ʱ��2�ص�");
}


void main_thread(void *pdata)
{
	int cnt = 0;
	#ifdef DEBUG
	RCC_ClocksTypeDef RCC_ClocksStatus;
	#endif
	
	driver_misc_init(); //��ʼ��һЩ����(�������)
	usr_gpio_init(); //��ʼ��GPIO
	//IND3_ON; //����LED
#ifdef OS_UCOS
#ifdef UCOS_V3
	OSStatTaskCPUUsageInit((OS_ERR*)&ret);
#else
	OSStatInit(); //��ʼ��UCOS״̬
#endif
#endif
	uart1_init(); //��ʼ������1
	uart3_init();

	//��ӡMCU����ʱ��
#ifdef DEBUG
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	p_dbg("SYSCLK_Frequency:%d,HCLK_Frequency:%d,PCLK1_Frequency:%d,PCLK2_Frequency:%d,ADCCLK_Frequency:%d\n", 
		RCC_ClocksStatus.SYSCLK_Frequency, 
		RCC_ClocksStatus.HCLK_Frequency, 
		RCC_ClocksStatus.PCLK1_Frequency, 
		RCC_ClocksStatus.PCLK2_Frequency, 
		0);
#endif

/*
*os��������֮�����ʱ��
*
*
*����������ʱ��,һ��һ���Զ�ʱ��,һ��ѭ����ʱ��
*������ע��init_timer_tasklet�������,�����뿴����ԭ��(λ��task.c)
*/
	init_timer_tasklet();		

	timer1 = timer_setup(1000, 	//��ʱʱ��
			0, 	//ֻ��ʱһ��
			test_timer_cb1,	//�ص�����
			"����1"); //����
	if(!timer1)
		p_err("timer1 ����ʧ��");
	else
		p_dbg("timer1 ������");
	
	timer2 = timer_setup(2000, 	//��ʱʱ��
			1, 	//ѭ����ʱ����
			test_timer_cb2,	//�ص�����
			"����2"); //����
	if(!timer1)
		p_err("timer2 ����ʧ��");
	else
		p_dbg("timer2 ������");
	
	add_timer(timer1);
	add_timer(timer2);
	
	sleep(10000);	//�ȴ�10s
	p_dbg("�޸�timer2 ��ʱʱ���1��");
	del_timer(timer2);   //del_timer��ֹͣ��ʱ��,timer_free��ɾ���ͷŶ�ʱ��
	mod_timer(timer2, 1000); //�޸Ķ�ʱʱ��Ϊ1s
	while(1)
	{
		sleep(2000);
		//p_dbg("%s %d", __FUNCTION__, cnt++);
	}
}

int main(void)
{
#ifdef OS_UCOS
#ifdef UCOS_V3
	OS_ERR err;
	OSInit(&err);
#else
	OSInit();
#endif
#endif
	_mem_init(); //��ʼ���ڴ����
	msg_q_init();//
	thread_create(main_thread, 0, TASK_MAIN_PRIO, 0, TASK_MAIN_STACK_SIZE, "main_thread");
#ifdef OS_UCOS
#ifdef UCOS_V3
	OSStart(&err);
#else
	OSStart();
#endif
#endif
#ifdef OS_FREE_RTOS
	vTaskStartScheduler();
#endif
	return 0;
}
