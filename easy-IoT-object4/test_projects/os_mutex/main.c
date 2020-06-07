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

mutex_t test_mutex = 0;

void test_thread1(void *pdata)
{
	int cnt = 0;
	p_dbg("�����߳�:%s", (char*)pdata);	
	while(cnt < 100)
	{
		sleep(10);  //�߳�1�����ȼ���һЩ,�����趨ÿ10ms��ӡһ��
		mutex_lock(test_mutex);  
		p_dbg("[1234567890]");  //��ӡ�������ͼ���
		mutex_unlock(test_mutex);
	}
	p_dbg("�˳��߳�:%s", (char*)pdata);	
	thread_exit(thread_myself());
}

void test_thread2(void *pdata)
{
	int cnt = 0;
	p_dbg("�����߳�:%s", (char*)pdata);	
	
	while(cnt < 100)
	{
		mutex_lock(test_mutex);    //��ס,����Խ��������ע�͵�����Ч��
		p_dbg("<abcdefghijklmnopqrstuvwxyz>");  //��ӡ�������ͼ���
		mutex_unlock(test_mutex);  //����
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
*os��������֮����
*
*
*���������̲߳��������ٶ�ѭ����ӡһ���ַ�,��ӡ100������˳��߳�
*
*/
	test_mutex = mutex_init("����"); //����һ���������
	if(!test_mutex)
		p_err("test_mutex ����ʧ��");
	else
		p_dbg("test_mutex ������");

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
