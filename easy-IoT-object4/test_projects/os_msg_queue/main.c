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

msg_q_t test_msg = 0;

void test_thread1(void *pdata)
{
	int cnt = 0;
	p_dbg("�����߳�:%s", (char*)pdata);	
	while(cnt < 5)
	{
		sleep(1000);  //�߳�1�����ȼ���һЩ,�����趨ÿ10ms��ӡһ��
		p_dbg("������Ϣ");
		msgsnd(test_msg, "�����߳�1����Ϣ");	
		cnt++;
	}
	p_dbg("�˳��߳�:%s", (char*)pdata);	
	thread_exit(thread_myself());
}

void test_thread2(void *pdata)
{
	int cnt = 0, ret;
	void *p_msg_recv;
	p_dbg("�����߳�:%s", (char*)pdata);	
	
	while(cnt < 10)
	{
		ret = msgrcv(test_msg, &p_msg_recv, 2000);
		if(ret < 0)
			p_err("test_event err");
		else if(ret > 0)
			p_dbg("TIMEOUT");
		else
			p_dbg("���յ���Ϣ:%s", (char*)p_msg_recv);
		cnt++;
	}
	p_dbg("�˳��߳�:%s", (char*)pdata);	
	thread_exit(thread_myself());
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
*os��������֮��Ϣ����
*
*
*���������߳�
*	�߳�2����˯��,��Ϣ����
*	�߳�1ÿ�뷢��һ����Ϣ���߳�2
*
*/
	cnt = msgget(&test_msg, 10); //����һ���������
	if(cnt)
		p_err("test_event ����ʧ��");
	else
		p_dbg("test_event ������");

	cnt = thread_create(test_thread1, 
		"����1", 	
		TASK_MAIN_PRIO + 1, 	//�߳����ȼ�,��ע��ucos2������ÿ���̵߳����ȼ�������ͬ
		0, 	//�̶߳�ջָ��,Ϊ0�����Զ�����
		256, //��ջ��С
		"�߳�����1");
	if(cnt < 0)
		p_err("�߳� 1 ����ʧ��");
	else
		p_dbg("�߳� 1 �����ɹ�,�߳�ID:%d", cnt);
		
	cnt = thread_create(test_thread2, 
		"����2", 
		TASK_MAIN_PRIO + 2, 
		0, 
		256, 
		"�߳�����2");
	if(cnt < 0)
		p_err("�߳� 2 ����ʧ��");
	else
		p_dbg("�߳� 2 �����ɹ�,�߳�ID:%d", cnt);
	
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
