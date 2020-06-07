#define DEBUG
#include "debug.h"
#include "drivers.h"
#include "app.h"
#include "api.h"


#include "cfg80211.h"
#include "defs.h"
#include "type.h"
#include "types.h"

#include "wifi.h"

#include "moal_main.h"
#include "moal_uap.h"
#include "moal_uap_priv.h"
#include "mlan_ioctl.h"

#include "lwip/netif.h"

#ifdef UAP_SUPPORT
extern int wifi_get_sta_list(mlan_ds_sta_list *p_list);
#endif
extern int wifi_get_stats(uint8_t *mac, struct station_info *sinfo);
extern int wifi_scan(void (*scan_result_callback)(struct scan_result_data *res_data), 
		char *essid); 


const char *wpa_proto[4] = {
	"NONE",
	"WPA",
	"WPA2/RSN",
	"OPEN/SHARE"
};

void print_scan_result(struct scan_result_data *dat)
{
	char *cipher = "";
	char *mode;
	
	char essid_name[33];

	if(dat->cipher & WPA_CIPHER_NONE)
		cipher = "NONE";
	else
	{
		if(dat->cipher & WPA_CIPHER_WEP40)
			cipher = "WEP40";
		if(dat->cipher & WPA_CIPHER_WEP104)
			cipher = "WEP104";
		if(dat->cipher & WPA_CIPHER_TKIP)
			cipher = "TKIP";
		if(dat->cipher & WPA_CIPHER_CCMP)
			cipher = "CCMP";
	}	

	if(dat->mode == MODE_ADHOC)
		mode = "AD-HOC";
	else if(dat->mode == MODE_AP)
		mode = "MASTER";
	else
		mode = "unkown";
	
	memcpy(essid_name,dat->essid,32);
	essid_name[32] = 0;
	if(dat->essid_len == 0)
	{
		essid_name[0] =0;
	}
	if(dat->essid_len < 32)
	{
		essid_name[dat->essid_len] =0;
	}
	
	p_dbg("\r\nAP:%d",dat->num);	
	p_dbg("	�ź�ǿ��:%ddbm",dat->rssi);
	p_dbg("	��֤����:%s",	(dat->proto <= 3)?wpa_proto[dat->proto]:"unkown");	
	p_dbg("	����ģʽ:%s", cipher);
	p_dbg("	Ƶ��:%d",dat->channel);	
	p_dbg("	Ƶ��:%dMHz",dat->freq);	
	p_dbg("	ģʽ:%s", mode);	
	p_dbg("	ssid:%s",essid_name);
	dump_hex("	bssid",dat->bssid,6);
}


void scan_result_fun(struct scan_result_data *res_data)
{
	if(res_data)
		print_scan_result(res_data);
}

char essid[] = {0xe9,0xad,0x85,0xe8,0x93,0x9d,0x20,0x6e,0x6f,0x74,0x65};

/*
*����ɨ��,����'1'
*/
void test_scan()
{
	p_dbg_enter;
	wifi_scan(scan_result_fun, "");
	p_dbg_exit;
}

/**
 *����WIFI����,����'2'
 *���ӵ�����Ϊ"TP-LINK_2AA6����·����������ģʽ��Ƶ���Զ���Ӧ
 *���볤����WPA��WPA2ģʽ��8 <= len <= 64;��WEPģʽ�±���Ϊ5��13
 */
void test_wifi_connect()
{
	char essid[] = "TP-LINK_2AA6";
	char *password = "zsl540535";
	p_dbg_enter;

	p_dbg("Connect to STA:%s, PWD:%s", essid, password);

	wifi_set_mode(MODE_STATION);

	wifi_connect(essid, password);

	if (is_wifi_connected())
	{
		p_dbg("wifi connect ok");
		netif_set_down(p_netif);
		netif_set_up(p_netif);
	}

	p_dbg_exit;

}


/*
 *����AP,����'4'
 *
 */
void test_create_ap()
{
	char *essid = "lwip_ap";
	char *password = "12345678";
	p_dbg_enter;
	p_dbg("����ap:%s, ����:%s", essid, password);

	wifi_set_mode(MODE_AP);

	wifi_ap_cfg(essid, password, KEY_WPA2, 6, 4);

	p_dbg_exit;
}

/*
 *���ԶϿ�WIFI����,����'3'
 *
 */
void test_wifi_disconnect()
{
	p_dbg_enter;
	wifi_disconnect();
	p_dbg_exit;
}


/*
 *���Լ���adhoc����,����'q'
 *
 */
void test_wifi_join_adhoc()
{
	p_dbg_enter;
	wifi_set_mode(MODE_STATION);  //ADHOC������staģʽ��
	wifi_set_mode(MODE_ADHOC);
	wifi_join_adhoc("xrf_adhoc", "1234567890abc", 6);
	p_dbg_exit;
}

/*
 *�����Ƴ�adhoc����,����'r'
 *
 */
void test_wifi_leave_adhoc()
{
	p_dbg_enter;
	wifi_leave_adhoc();
	p_dbg_exit;
}



/*
 *���Ի�ȡWIFI������Ϣ,������ж���ͻ�������,������Ҫָ�����ַ
 *�����test_get_station_list����ȡ���һ��mac��ַ��Ϊ����
 *����ִ�д˲���֮ǰ����ִ��test_get_station_list
 *,����'f'
 */
uint8_t sta_mac[8] = {0,0,0,0,0,0};
void test_wifi_get_stats()
{
	int ret;
	struct station_info sinfo;
	
	ret = wifi_get_stats(sta_mac, &sinfo);

	if(ret == 0)
		p_dbg("wifi stats, rssi:%d", sinfo.signal);
	else
		p_err("test_wifi_get_stats err");
}

/*
 *�ر�AP,����'n'
 *
 */

void test_stop_ap()
{
	p_dbg_enter;
	wifi_stop_ap();
	p_dbg_exit;
}

/*
 *���Ի�ȡapģʽ�������ӵĿͻ�����Ϣ,����'o'
 *
 */
void test_get_station_list()
{
#ifdef UAP_SUPPORT
	int i;
	int rssi = 0xffffff00;
	mlan_ds_sta_list *p_list;
	p_dbg_enter;
	p_list = (mlan_ds_sta_list *)mem_calloc(sizeof(mlan_ds_sta_list), 1);
	if(p_list){
		wifi_get_sta_list(p_list);

		for(i = 0; i < p_list->sta_count; i++)
		{
			p_dbg("sta %d", i);
			rssi = rssi | p_list->info[i].rssi;
			p_dbg("	rssi %d", rssi);
			dump_hex("	mac", p_list->info[i].mac_address, 6);

			memcpy(sta_mac, p_list->info[i].mac_address, 6);
		}
	}
#endif
	p_dbg_exit;
}


void test_power_save_enable()
{
	p_dbg("enter power save");
	wifi_power_cfg(2);
}

void test_power_save_disable()
{
	p_dbg("exit power save");
	wifi_power_cfg(0);
}


