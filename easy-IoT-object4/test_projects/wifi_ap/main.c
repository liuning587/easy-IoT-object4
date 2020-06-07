#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "cfg80211.h"
#include "defs.h"
#include "type.h"
#include "types.h"

#include "moal_sdio.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "wpa.h"

#include "moal_main.h"
#include "moal_uap.h"
#include "moal_uap_priv.h"
#include "mlan_ioctl.h"

/*
* �汾˵����docĿ¼
*/
#define VIRSION		"V2.4"

int read_firmware(u32 offset, u32 len, u8 * pbuf)
{
	p_err_miss;
	return 0;
}

char command;
int dbg_level = 1;
int monitor_enable = 0;

struct netif *p_netif = (struct netif *)1;

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

/*
*�պ���,����������
*/
void dev_monitor_task(void *arg)
{
	while(1)
		sleep(1000);
}

/*
*����wifi�������ݵĽӿ�,����пͻ������ӵ���ap,�ᷢ��tcpip������ݰ�����
*������16���ƴ�ӡ
*/
void ethernetif_input(struct netif *netif,void *p_buf,int size)
{
	//p_dbg("recv %d byte", size);
	//dump_hex("data", p_buf, size);
}

void main_thread(void *pdata)
{
	int ret;
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
*WIFI����֮����AP
*������16���ƴ�ӡ
*/

	init_work_thread();	//��ʼ�������߳�
	
	ret = init_wifi();//��ʼ��WIFIоƬ
	if(ret == 0)
	{
		//init_monitor(); //��ʼ��monitorģ��,������init_wifi֮�����
	
	}else{
		p_err("init wifi faild!"); 
	}

	wifi_set_mode(MODE_AP);
	wifi_ap_cfg("xrf_ap", 	//ssid
		"12345678", 	//key
		KEY_WPA2, 		//���ܷ�ʽ,��ѡֵKEY_NONE, KEY_WEP, KEY_WPA, KEY_WPA2
		6, 	//Ƶ��,��ѡ1-13
		4		//�������Ŀͻ���1-6
		);
	
	//IND3_OFF; 
	//����ÿ3���Ӳ�ѯһ�οͻ������
	while(1)
	{
		int i;
		int rssi = 0xffffff00;
		mlan_ds_sta_list *p_list;

		p_list = (mlan_ds_sta_list *)mem_calloc(sizeof(mlan_ds_sta_list), 1);
		if(p_list){
			wifi_get_sta_list(p_list);
			p_dbg("�������豸��:%d", p_list->sta_count);
			for(i = 0; i < p_list->sta_count; i++)
			{
				p_dbg("sta %d", i);
				rssi = rssi | p_list->info[i].rssi;
				p_dbg("	rssi %d", rssi);
				dump_hex("	mac", p_list->info[i].mac_address, 6);

				//ͨ��mac��ַ,���ǿ��Ի�ȡstation�ĸ�����Ϣ
				{
					struct station_info sinfo;
		
					ret = wifi_get_stats(p_list->info[i].mac_address, &sinfo);
					if(ret == 0)
						p_dbg("wifi stats, rssi:%d", sinfo.signal);
				}
			}
			
			mem_free(p_list);
		}
		p_dbg("\r\n\r\n\r\n");

		sleep(3000);
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
