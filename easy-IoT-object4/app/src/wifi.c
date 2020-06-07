/*
 * 
 *
 */
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

//#include "wifidirectutl.h"

/*
*��Ϊstation��ap�����豸,��ӿں����ǲ�һ����
*�����ڲ�ͬ��ģʽ������ѡ�����Ӧ���豸���в���
*wifi_devָ��ǰ�豸
*/
struct net_device *sta_wifi_dev = NULL;
struct net_device *ap_wifi_dev = NULL;
struct net_device *direct_wifi_dev = NULL;

struct net_device *wifi_dev = NULL;

void wifi_init_priv(void)
{
    moal_private *priv;
	struct wireless_dev	*tmp;
	
    if(sta_wifi_dev != NULL)
    {
        priv = (moal_private *)sta_wifi_dev->ml_priv;
		tmp = sta_wifi_dev->ieee80211_ptr;
        priv->wdev = tmp;
		
		woal_init_priv(priv, MOAL_CMD_WAIT);
	}

	if(ap_wifi_dev != NULL)
    {
        priv = (moal_private *)ap_wifi_dev->ml_priv;
		tmp = ap_wifi_dev->ieee80211_ptr;
        priv->wdev = tmp;
		
		woal_init_priv(priv, MOAL_CMD_WAIT);
	}

}

struct net_device *get_wifi_dev()
{
	return wifi_dev;
}

int is_hw_ok()
{
	return wifi_dev?1:0;
}


int wifi_get_mode()
{
	moal_private *priv;
	priv = (moal_private *)wifi_dev->ml_priv;
	if(priv->bss_role == MLAN_BSS_ROLE_UAP)
		return MODE_AP;
	else
		return MODE_STATION;
}
/**
 * @brief  ����wifiģʽ,ֻ���л�ģʽ,����ap������������
 * @param  type : MODE_AP, MODE_STATION, MODE_ADHOC
 * @retval ok: 0, err: -1
 */
int wifi_set_mode(int type)
{
	int ret = -1;
	int set_type;
	moal_private *priv;
	struct net_device *saved_dev = wifi_dev;

	priv = (moal_private *)wifi_dev->ml_priv;
	//�ݲ�֧��ap+staͬʱ���ڣ��л�ģʽǰ�ȹر�wifi
	if(priv->bss_role == MLAN_BSS_ROLE_UAP)
		wifi_stop_ap();
	else
		wifi_disconnect();
	if(0)
		;
#ifdef UAP_SUPPORT		
	else if(type == MODE_AP){
		set_type = NL80211_IFTYPE_AP;
		wifi_dev = ap_wifi_dev;
	}
#endif
#ifdef STA_SUPPORT	
	else if(type == MODE_STATION){
		wifi_dev = sta_wifi_dev;
		set_type = NL80211_IFTYPE_STATION;
	}

	else if(type == MODE_ADHOC){
		wifi_dev = sta_wifi_dev;
		set_type = NL80211_IFTYPE_ADHOC;
	}
#endif
#ifdef WIFI_DIRECT_SUPPORT	
	else if(type == MODE_P2P_GO){
		wifi_dev = direct_wifi_dev;
		set_type = NL80211_IFTYPE_P2P_GO;
	}
	else if(type == MODE_P2P_GC){
		wifi_dev = direct_wifi_dev;
		set_type = NL80211_IFTYPE_P2P_CLIENT;
	}
#endif
	else{
		p_err("type %d not support", type);
		return -1;
	}

	if(wifi_dev == NULL){
		
		wifi_dev = saved_dev;
		p_err("%s not support", __FUNCTION__);
		return -1;
	}

	if(wifi_dev->cfg80211_ops->change_virtual_intf){
		priv = (moal_private *)wifi_dev->ml_priv;
		p_dbg("change_virtual_intf");
		//ap��staָ����ͬ��change_virtual_intf
		ret = wifi_dev->cfg80211_ops->change_virtual_intf(priv->wdev->wiphy, wifi_dev, (enum nl80211_iftype)set_type, 0, 0);
		if(ret != 0)
		{
			wifi_dev = saved_dev;
		}
	}else
		p_err("%s not support", __FUNCTION__);

	//lwip_netif_init(); 

	return ret;
}


/**
 * @brief  ����Ӳ���Ķಥ���㲥������ģʽ
 * @param  flags :  IFF_PROMISC	�������еİ�
				IFF_ALLMULTI �������еĶಥ��(��ʱ���ý������жಥ���ļ򵥷�ʽ��
				������ಥ��ַ��)

 * @retval ok: 0, err: -1
 */
int wifi_set_multicast(uint32_t flags)
{
	if(!wifi_dev)
		return -1;
	
	wifi_dev->flags |= flags;

	if(wifi_dev->netdev_ops && wifi_dev->netdev_ops->ndo_set_multicast_list)
		wifi_dev->netdev_ops->ndo_set_multicast_list(wifi_dev);
	
	return 0;
}


/**
 * @brief  ����Ϊ����ģʽ
 * @param  
 * @retval ok: 0, err: -1
 */
int wifi_set_promisc()
{
	return wifi_set_multicast(IFF_PROMISC);
}


/**
 * @brief  ����wifiƵ��,for ap
 * @param  channel : 1 - 14
 * @retval ok: 0, err: -1
 */
int wifi_set_channel(int channel)
{
	int ret = -1;
	struct ieee80211_channel ch;
	moal_private *priv;

	//sta��ap��cfg80211_ops�ӿ��ǲ�һ����,���������Ա�Ƿ���Ч,��ͬ
	if(wifi_dev->cfg80211_ops->set_channel){
		priv = (moal_private *)wifi_dev->ml_priv;

		memset(&ch, 0, sizeof(struct ieee80211_channel));
		ch.hw_value = channel;
		ch.band = IEEE80211_BAND_2GHZ;
		ch.center_freq = ieee80211_channel_to_frequency(ch.hw_value, ch.band);
		
		//ap��staָ����ͬ��change_virtual_intf
		ret = wifi_dev->cfg80211_ops->set_channel(priv->wdev->wiphy, wifi_dev, &ch, NL80211_CHAN_NO_HT);
	}else
		p_err("%s not support", __FUNCTION__);
	
	return ret;
}


/**
 * @brief  ����wifiģʽΪAP����
 *
 * @param  name : ��������
 * @param  key:����
 * @param  key_type:����ģʽ,��ѡ����KEY_NONE, KEY_WEP,KEY_WPA,KEY_WPA2
 * @param  channel:Ƶ��
 * @param  max_client:�������ͻ�������,1 - MAX_STA_COUNT(SDKԤ��Ϊ10)
 *
 * @retval ok: 0, err: -1
 */
int wifi_ap_cfg(char *name, char *key, int key_type, int channel, int max_client)
{
	int ret = -1;
	char str_tmp[64];
	/* example
	char *cmd = "ASCII_CMD=AP_CFG,"\
	"SSID=QWERT,"\
	"SEC=wep128,"\
	"KEY=12345,"\
	"CHANNEL=8,"\
	"MAX_SCB=4,"\
	"END";
	*/

	//����ap��������û��ʹ��cfg80211_ops�ṩ��, ����ʹ�ø��򵥵�netdev_ops�ӿ�
	//netdev_ops�ӿڱȽϷḻ,������һ���ο�
	if(wifi_dev->netdev_ops && wifi_dev->netdev_ops->ndo_do_ioctl)
	{
		struct iwreq *wrq = (struct iwreq *)mem_calloc(256, 1);

		if(!wrq)
			return ret;
		
		wrq->u.data.pointer = wrq + 1;
		strcpy(wrq->u.data.pointer, "ASCII_CMD=AP_CFG,");

		snprintf(str_tmp, 64, "SSID=%s,", name);
		strcat(wrq->u.data.pointer, str_tmp);

		if (key_type == KEY_NONE)
			strcpy(str_tmp, "SEC=open,");
		else if (key_type == KEY_WEP)
			strcpy(str_tmp, "SEC=wep128,");
		else if (key_type == KEY_WPA)
			strcpy(str_tmp, "SEC=wpa-psk,");
		else if (key_type == KEY_WPA2)
			strcpy(str_tmp, "SEC=wpa2-psk,");
		else
			strcpy(str_tmp, "SEC=open,");
		
		strcat(wrq->u.data.pointer, str_tmp);

		snprintf(str_tmp, 64, "KEY=%s,", key);
		strcat(wrq->u.data.pointer, str_tmp);

		snprintf(str_tmp, 64, "CHANNEL=%d,", channel);
		strcat(wrq->u.data.pointer, str_tmp);

		snprintf(str_tmp, 64, "MAX_SCB=%d,", max_client);
		strcat(wrq->u.data.pointer, str_tmp);

		strcat(wrq->u.data.pointer, "END");

		wrq->u.data.length = strlen(wrq->u.data.pointer);
		
		ret = wifi_dev->netdev_ops->ndo_do_ioctl(wifi_dev, (void*)wrq, WOAL_UAP_FROYO_AP_SET_CFG);
		if(ret == 0)
			ret = wifi_dev->netdev_ops->ndo_do_ioctl(wifi_dev, 0, WOAL_UAP_FROYO_AP_BSS_START);

		mem_free(wrq);

	}else
		p_err("%s not support", __FUNCTION__);
	
	return ret;
}


int wifi_stop_ap()
{
	int ret = -1;
	p_dbg_enter;
	if(wifi_dev->netdev_ops && wifi_dev->netdev_ops->ndo_do_ioctl)
	{
		ret = wifi_dev->netdev_ops->ndo_do_ioctl(wifi_dev, 0, WOAL_UAP_FROYO_AP_BSS_STOP);
	}
	p_dbg_exit;
	return ret;
}


/**
 * @brief 
 */
int wifi_join_adhoc(char *ssid, char *key, int ch)
{
	int ret = -1;
	moal_private *priv;
	struct cfg80211_ibss_params params;
	struct ieee80211_channel channel;
	int key_len = strlen(key);
	
	memset(&params, 0, sizeof(struct cfg80211_ibss_params));
	memset(&channel, 0 ,sizeof(struct ieee80211_channel));

	params.channel = &channel;
	
	params.beacon_interval = 100;
	params.ssid = (u8*)ssid;
	params.ssid_len = strlen(ssid);
	params.channel_fixed = TRUE;
	params.channel->band = IEEE80211_BAND_2GHZ;
	params.channel->hw_value = ch;
	params.channel->center_freq = ieee80211_channel_to_frequency(params.channel->hw_value, 
		params.channel->band);
	if(key_len) 
	{
		params.key = key;
		params.key_len = key_len;
		params.privacy = TRUE;	//ʹ��wep����
	}
	
	priv = (moal_private *)wifi_dev->ml_priv;
	if(wifi_dev->cfg80211_ops && wifi_dev->cfg80211_ops->join_ibss)
	{
		ret = wifi_dev->cfg80211_ops->join_ibss(priv->wdev->wiphy, wifi_dev, &params);

	}else
		p_err("%s not support", __FUNCTION__);
	
	return ret;
}


int wifi_leave_adhoc()
{
	int ret = -1;
	moal_private *priv;
	
	priv = (moal_private *)wifi_dev->ml_priv;
	if(wifi_dev->cfg80211_ops && wifi_dev->cfg80211_ops->leave_ibss)
	{
		ret = wifi_dev->cfg80211_ops->leave_ibss(priv->wdev->wiphy, wifi_dev);

	}else
		p_err("%s not support", __FUNCTION__);
	
	return ret;
}

#ifdef UAP_SUPPORT
int wifi_get_sta_list(mlan_ds_sta_list *p_list)
{
	int ret = -1;
	
	if(wifi_dev->netdev_ops && wifi_dev->netdev_ops->ndo_do_ioctl)
	{
		struct iwreq *wrq = (struct iwreq *)mem_calloc(sizeof(struct iwreq), 1);
		if(wrq)
		{
			wrq->u.data.pointer = p_list;
		
			ret = wifi_dev->netdev_ops->ndo_do_ioctl(wifi_dev, (void*)wrq, UAP_GET_STA_LIST);

			mem_free(wrq);
		}
		
	}else
		p_err("%s not support", __FUNCTION__);
	
	return ret;
	
}
#endif

struct cfg80211_scan_request *new_scan_req(moal_private *priv, char *ssid, u8_t ssid_len)
{
	struct wiphy *wiphy = priv->wdev->wiphy;
	struct cfg80211_scan_request *creq = NULL;
	int i, n_channels = 0;
	enum ieee80211_band band;

	for (band = (enum ieee80211_band)0; band < IEEE80211_NUM_BANDS; band++) {
		if (wiphy->bands[band])
			n_channels += wiphy->bands[band]->n_channels;
	}

	p_dbg("_new_connect_scan_req,channels:%d,creq_size:%d\n",n_channels,sizeof(*creq) + sizeof(struct cfg80211_ssid) +
		       n_channels * sizeof(void *));
	creq = (struct cfg80211_scan_request *)mem_calloc(sizeof(*creq) + sizeof(struct cfg80211_ssid) +
		       n_channels * sizeof(void *),1);
	if (!creq)
		return NULL;
	/* SSIDs come after channels */
	creq->wiphy = wiphy;
	creq->ssids = (void *)&creq->channels[n_channels];
	creq->n_channels = n_channels;
	if(ssid_len)
		creq->n_ssids = 1;

	/* Scan all available channels */
	i = 0;
	for (band = (enum ieee80211_band)0; band < IEEE80211_NUM_BANDS; band++) {
		int j;

		if (!wiphy->bands[band])
			continue;

		for (j = 0; j < wiphy->bands[band]->n_channels; j++) {
			/* ignore disabled channels */
			if (wiphy->bands[band]->channels[j].flags &
						IEEE80211_CHAN_DISABLED)
				continue;

			creq->channels[i] = &wiphy->bands[band]->channels[j];
			i++;
		}
	}
	if (i) {
		/* Set real number of channels specified in creq->channels[] */
		creq->n_channels = i;

		/* Scan for the SSID we're going to connect to */
		memcpy(creq->ssids[0].ssid, ssid, ssid_len);
		creq->ssids[0].ssid_len = ssid_len;
	} else {
		/* No channels found... */
		mem_free(creq);
		creq = NULL;
	}

	return creq;
}


/**
 * @brief  ɨ�踽����AP,usr_scan_result_callback�������Զ����һ���ص��ӿ�
 *		ÿɨ�赽һ��ap�ͻ���wlan_ret_802_11_scan->handle_user_scan_result����һ��,�����scan_result_data����һЩ����
 *		��ap��Ϣ,һ���Ѿ�����,���Ҫ����ϸ��ap����Ϣ��ֱ����handle_user_scan_result����ȡ
 * @param
 * @retval ok: 0, err: -1
 */
extern void (*usr_scan_result_callback)(struct scan_result_data *res_data);

int wifi_scan(void (*scan_result_callback)(struct scan_result_data *res_data), 
		char *essid)
{
	moal_private *priv;
	struct cfg80211_scan_request *scan_request;
	
	if(!wifi_dev)
		return -1;
	priv = (moal_private *)wifi_dev->ml_priv;

	if(priv->bss_role == MLAN_BSS_ROLE_UAP)
	{
		p_err("APģʽ�²�֧��scan�������л���STAģʽ");
		return 0;
	}
	
	usr_scan_result_callback = scan_result_callback;
	scan_request = new_scan_req(priv, essid, strlen(essid));
	wifi_dev->cfg80211_ops->scan(priv->wdev->wiphy, wifi_dev, scan_request);

	//scan���첽ʵ�ֵ�,���������������˳�
	//ɨ����ɺ��������Զ�����cfg80211_scan_done����

	return 0;
}


/**
 * @brief  wifi �Ƿ��Ѿ�����
 *
 */
int is_wifi_connected()
{

	int ret = 0;
	moal_private *priv;
	priv = (moal_private *)wifi_dev->ml_priv;
	
	if(priv->media_connected)
		ret = 1;

	return ret;
}

/**
 * @brief  wifi����,�����Զ��������ģʽ��Ƶ��,�û�ֻ���ṩ·�������ƺ�����
 * @param
 *							
 *		essid: ��AP����
 *		key: ��������	
 * @retval ok: 0, err: -1
 */
int wifi_connect( char *essid, char *key)
{

	int result_ret = 0;
	moal_private *priv;
	int essid_len = strlen(essid);
	int key_len = strlen(key);
	struct cfg80211_connect_params *smee = 0;
	
	p_dbg("enter %s\n", __FUNCTION__);
	if(essid_len > 32 || key_len > 32)
	{
		result_ret = -1;
		goto ret;
	}
	smee = (struct cfg80211_connect_params *)mem_malloc(sizeof(struct cfg80211_connect_params));
	if(smee == 0)
	{
		result_ret = -1;
		goto ret;
	}

	priv = (moal_private *)wifi_dev->ml_priv;
	memset(smee,0,sizeof(struct cfg80211_connect_params));
	smee->bssid = 0; 
	
	memcpy(smee->ssid,essid,essid_len);
	smee->ssid[essid_len] = 0;
	
	smee->ssid_len = essid_len;
	
	memcpy(smee->key,key,key_len);
	smee->key[key_len] = 0;
	
	smee->key_len = key_len;
	smee->auth_type = NL80211_AUTHTYPE_AUTOMATIC;
	smee->crypto.wpa_versions = NL80211_WPA_VERSION_2;
	smee->crypto.cipher_group = MLAN_ENCRYPTION_MODE_CCMP;
	smee->crypto.n_ciphers_pairwise = 1;
	smee->crypto.ciphers_pairwise[0] = MLAN_ENCRYPTION_MODE_CCMP;

/*
 *smee�ṹֻ����˲��ֳ�Ա��������Ա�������ӵ�ʱ���Զ����(����Ŀ��ap����Ϣ)
*/
	
	result_ret = wifi_dev->cfg80211_ops->wifi_connect(priv->wdev->wiphy, wifi_dev,smee);
ret:
	if(smee)
		mem_free(smee);
	if(result_ret)
		p_err("user_link_app err:%d\n",result_ret);

	p_dbg("exit %s\n", __FUNCTION__);
	
	return result_ret;
}

/**
 * @brief  �Ͽ�����
 */
int wifi_disconnect()
{
	int ret = -1;
	moal_private *priv;
	
	priv = (moal_private *)wifi_dev->ml_priv;
	if(wifi_dev->cfg80211_ops && wifi_dev->cfg80211_ops->disconnect)
	{
		ret = wifi_dev->cfg80211_ops->disconnect(priv->wdev->wiphy, wifi_dev, 0);

	}else
		p_err("%s not support", __FUNCTION__);
	
	
	return ret;
}


/**
 * @brief  ��ȡmac��ַ��mac��ַ��8782оƬ��ȡ
 * @param  mac_addr : mac��ַ 6BYTE
 */
int wifi_get_mac(unsigned char *mac_addr)
{
	
	memcpy(mac_addr, wifi_dev->dev_addr, ETH_ALEN);
	return 0;
}


/**
 * @brief  ��ȡWIFI���ӵ�ͳ����Ϣ(�ź�ǿ��...)
 * @param  pStats : ָ��station_info��ָ��
 * @param  mac : ָ����Ҫ��ȡ��station�ĵ�ַ
 */
int wifi_get_stats(uint8_t *mac,struct station_info *sinfo)
{
	int ret = -1;
	moal_private *priv;
	
	priv = (moal_private *)wifi_dev->ml_priv;
	if(wifi_dev->cfg80211_ops && wifi_dev->cfg80211_ops->get_station)
	{
		ret = wifi_dev->cfg80211_ops->get_station(priv->wdev->wiphy, wifi_dev, mac,sinfo);

	}else
		p_err("%s not support", __FUNCTION__);
	
	return ret;
}

/**
 * @brief  ����ʡ��ģʽ,�ڱ������ӵ�����¿ɴﵽ�ܿɹ�ʡ��Ч��
 *		ʡ��ģʽ�µĺĵ����������շ�������
 *		�����û��������ø���ϸ�ڵĵ�Դ�������,����DTIM���
 *		һ�������ʹ�ô˺����Ѿ�����
 *
 * @param  power_save_level : ȡֵ 0,1,2; 
 *	0����ر�ʡ��ģʽ(CAM)
 *	1����DTIMʡ�緽ʽ
 *    2����INACTIVITYʡ�緽ʽ
 * @param  mac : ָ����Ҫ��ȡ��station�ĵ�ַ
 */
int wifi_power_cfg(uint8_t power_save_level)
{
	int ret = -1;
	moal_private *priv;

	priv = (moal_private *)wifi_dev->ml_priv;
	if(priv->bss_role == MLAN_BSS_ROLE_UAP)
	{
		if(wifi_dev->netdev_ops && wifi_dev->netdev_ops->ndo_do_ioctl)
		{
			struct iwreq *wrq = (struct iwreq *)mem_calloc(sizeof(struct iwreq), 1);
			if(wrq)
			{
				/*
				if(sleep)
				{
					deep_sleep_para param;
					memset(&param, 0, sizeof(deep_sleep_para));
					wrq->u.data.pointer = &param;
					param.subcmd = UAP_DEEP_SLEEP;
					param.action = MLAN_ACT_SET;
					param.deep_sleep = TRUE;
					param.idle_time = 1000;
					ret = wifi_dev->netdev_ops->ndo_do_ioctl(wifi_dev, (void*)wrq, UAP_IOCTL_CMD);
	
				}else
				*/
				{
					mlan_ds_ps_mgmt ps_mgmt;

					memset(&ps_mgmt, 0, sizeof(mlan_ds_ps_mgmt));
					wrq->u.data.pointer = &ps_mgmt;

					ps_mgmt.flags = PS_FLAG_PS_MODE;
					if(power_save_level == 0)
						ps_mgmt.ps_mode = PS_MODE_DISABLE;
					else if(power_save_level == 1)
						ps_mgmt.ps_mode = PS_MODE_PERIODIC_DTIM;
					else if(power_save_level == 2)
						ps_mgmt.ps_mode = PS_MODE_INACTIVITY;
					else
						ps_mgmt.ps_mode = PS_MODE_DISABLE;
					
					ret = wifi_dev->netdev_ops->ndo_do_ioctl(wifi_dev, (void*)wrq, UAP_POWER_MODE);
				}

				mem_free(wrq);
			}
		}
	}

	if(priv->bss_role == MLAN_BSS_ROLE_STA)
	{
		if(wifi_dev->cfg80211_ops && wifi_dev->cfg80211_ops->set_power_mgmt)
		{
			bool enable;
			if(power_save_level == 0)
				enable = 0;
			else
				enable = 1;
			ret = wifi_dev->cfg80211_ops->set_power_mgmt(priv->wdev->wiphy, wifi_dev,enable, 0); 
		}
	}
	
	return ret;
}



/**
 * @brief  ���ﲻ���ṩWIFI�¼��Ļص�
 *
 * apģʽ����ο�wlan_ops_uap_process_event����
 * staģʽ����ο�wlan_ops_sta_process_event����
 */

int event_callback(int type)
{

	return 0;
}

extern uint8_t g_mac_addr[6];


extern struct netif if_wifi;
int register_netdev(struct net_device *dev)
{
	moal_private *priv;

	priv = (moal_private *)dev->ml_priv;
	if(0)
		;
#ifdef STA_SUPPORT	
	else if(priv->bss_type == MLAN_BSS_TYPE_STA)
		sta_wifi_dev = dev;
#endif
#ifdef UAP_SUPPORT	
	else if(priv->bss_type == MLAN_BSS_TYPE_UAP)
		ap_wifi_dev = dev;
#endif
#ifdef WIFI_DIRECT_SUPPORT	
	else if(priv->bss_type == MLAN_BSS_TYPE_WIFIDIRECT)
		direct_wifi_dev = dev;
#endif
	else
		p_err("unkown bss_role");
	
	wifi_dev = dev;
	return 0;
}


//extern struct lbs_private *if_sdio_probe(void);
extern int woal_init_module(struct sdio_func *func);
extern char *mac_addr;
int init_wifi()
{
	int ret;

	func.vendor = MARVELL_VENDOR_ID;
#if WIFI_CHIP == 8782
	func.device = SD_DEVICE_ID_8782;
#elif WIFI_CHIP == 8801
	func.device = SD_DEVICE_ID_8801;
#else 
	#error "��ѡ��WIFIоƬ�ͺ�"
#endif
	ret = SD_Init();    //��ʼ��SDIO�豸
		
	if (ret)
		return ret;

	//mac_addr = "00:4c:35:12:34:56"; ����������Լ������mac��ַ

	ret = woal_init_module(&func);
	wifi_init_priv();
	return ret;
}



