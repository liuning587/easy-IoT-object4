#define DEBUG

#include "drivers.h"
#include "app.h"
#include "api.h"

#include "test.h"
#include "dhcpd.h"
#include "web_cfg.h"
#include <cctype>

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
	p_dbg("recv %d byte", size);
	dump_hex("data", p_buf, size);
}

const uint8_t test_buff[] = {
//������һ���㲥��	
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 	//����Ŀ�ĵ�ַ
	0x40, 0x8d, 0x5c, 0x36, 0xee, 0x06, 	//����Դ��ַ,����Ӧ����ص�mac��ַ,�����������һ��
	0x08, 0x06,  //IP��������,����ΪARP��
	1,2,3,4,5,6		//�������ŵ�����
};

void mac_data_xmit()
{
	struct sk_buff *tx_skb;
	struct pbuf *q;
	char *p802x_hdr;
	
	if(!is_hw_ok())
		return;

	tx_skb = alloc_skb(sizeof(test_buff) + MLAN_MIN_DATA_HEADER_LEN + sizeof(mlan_buffer), 0);
	if(tx_skb == 0)
		return;

	skb_reserve(tx_skb, MLAN_MIN_DATA_HEADER_LEN + sizeof(mlan_buffer));
	
	p802x_hdr = (char*)tx_skb->data;

	memcpy(p802x_hdr, test_buff, sizeof(test_buff));

	tx_skb->len = sizeof(test_buff);
	p802x_hdr = (char*)tx_skb->data;

	//dump_hex("s", tx_skb->data, tx_skb->len);

	if(wifi_dev && wifi_dev->netdev_ops)
		wifi_dev->netdev_ops->ndo_start_xmit((void*)tx_skb, wifi_dev);
	
	return;
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
*WIFI����֮adhoc,����֯�Ե���,����ҪAP����ʵ������(����)�豸��ͨ��)
*
*��Ҫ������Ӳ��ܲ���,����һ�����+���Ե�WIFI����Ҳ������
*������ӷֱ��ϵ����Զ�����,����ÿ��������Է�����һ�����ݰ�,����û���õ�tcpipЭ��ջ
*�������ݰ����ݲ�����ȫ����tcpipЭ��
*/

	init_work_thread();	//��ʼ�������߳�
	
	ret = init_wifi();//��ʼ��WIFIоƬ
	if(ret == 0)
	{
		//init_monitor(); //��ʼ��monitorģ��,������init_wifi֮�����
	
	}else{
		p_err("init wifi faild!"); 
	}
	p_dbg("����WIFIģʽΪadhoc");
	wifi_set_mode(MODE_STATION);
	wifi_set_mode(MODE_ADHOC);
	wifi_join_adhoc("test_adhoc", //��������
	"1234567890abc", 	//����,ע�����볤��ֻ��5��13�ֽ�
	6);

	//IND3_OFF; 
	
	while(1)
	{
		sleep(2000);
		mac_data_xmit();
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
