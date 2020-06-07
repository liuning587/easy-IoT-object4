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

void test_thread1(void *pdata)
{
	int cnt = 0;
	p_dbg("�����߳�:%s", (char*)pdata);	
	sleep(500); //��ʱ500ms
	while(cnt < 100)
	{
		sleep(1000);
		p_dbg("%s %d", __FUNCTION__, cnt++);  //��ӡ�������ͼ���
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
		sleep(1000);
		p_dbg("%s %d", __FUNCTION__, cnt++);  //��ӡ�������ͼ���
	}
	p_dbg("�˳��߳�:%s", (char*)pdata);	
	thread_exit(thread_myself());
}

void main_thread(void *pdata)
{
	int cnt = 0;
	char *p_m;
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
*os��������֮�ڴ����
*
*
*
*
*/
//����һ��150byte���ڴ�, ��ע��ʵ�ʷ����ʱ����4byte�����
	p_m = mem_malloc(150);
	
	p_dbg("�ڴ���1,����:%d, ʣ��:%d", mem_get_size(), mem_get_free());
	p_dbg("�ڴ���2,����:%d, ʣ��:%d", mem_get_size2(), mem_get_free2());
	
//�������һ��1000byte���ڴ�,Ȼ���ͷ�
	p_m = mem_malloc(1000);
	if(!p_m)
		p_err("no memory");
	else{
		
		p_dbg("start show memory");
		mem_slide_check(1);	//��ӡ�ڴ�������, ���ǿ��Կ����������������ڴ�
		p_dbg("end show memory");
		
		p_dbg("free it");
		mem_free(p_m);
	}
	

//��������ڴ�������
//����һ��64byte������,Ȼ��������е�65byte��ֵ,�������ڻ��ӡʲô��Ϣ
	p_m = mem_malloc(64);
	memset(p_m, 4, 65);
	
	while(1)
	{
		sleep(1000);
		
		p_dbg("%s %d", __FUNCTION__, cnt++);
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
