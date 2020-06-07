#ifndef _AUDIO_H
#define _AUDIO_H

#include "api.h"
#ifdef HAVE_CONFIG_H
#include "speex_config.h"
#endif
#include <speex/speex.h>		
#include "speex_arch.h"


#define AUDIO_TYPE_PCM			0
#define AUDIO_TYPE_ADPCM		1
#define AUDIO_TYPE_MP3 			2
#define AUDIO_TYPE_SPEEX		3
#define AUDIO_TYPE_UNKOWN 		4

#define AUDIO_TYPE 	AUDIO_TYPE_SPEEX

#define SPEEX_DEFAULT_QUALITY	2

/* quality  
     * 1 : 4kbps 10byte(very noticeable artifacts, usually intelligible)  
     * 2 : 6kbps 15byte(very noticeable artifacts, good intelligibility)  
     * 4 : 8kbps 20byte(noticeable artifacts sometimes)  
     * 6 : 11kpbs 28byte(artifacts usually only noticeable with headphones)  
     * 8 : 15kbps 38byte(artifacts not usually noticeable)  
     */ 

#define SPEEX_ENC_FRAME_SIZE	160
#define SPEEX_DEC_FRAME_SIZE	audio_cfg.dec_frame_size	//160������ֵѹ����Ĵ�С������ѹ�������᲻һ��

#define MAX_BLANK_BYTES				256

//DMA BUFF��������Ҫ���ŵ�����(�����뵽DAC������)���ֵ��Ҫ���ݲ����ʴ�С���е���
//���̫С���������
#define AUDIO_DMA_BUFF_SIZE 		3200	

#define AUDIO_DAC_INPUT_BUFF_SIZE  	3200	//speex�ò���

#define AUDIO_ADC_BUFF_SIZE  		6400//3200
//for samplate 16000, we have 32000 bytes every second
//this buffer reserves for sound for 1.5s (1.5*2byte*16000)
#ifdef OS_FREE_RTOS
#define AI_VOICE_LEN                           (AUDIO_SAMPLERATE16000)
#else
#define AI_VOICE_LEN                           (AUDIO_SAMPLERATE16000*3)
#endif

struct AUDIO_DEC_INPUT
{
	uint8_t old_type;
	uint8_t type;
	uint8_t bits;
	uint8_t channels;
	unsigned int samplerate;
	uint8_t *data;
	uint32_t data_len;
	uint32_t buff_len;
};

//dec�������,ͬʱҲ��dac���뻺��
struct AUDIO_DEC_OUTPUT
{
	uint32_t len;
	uint32_t write_circle_cnt;
	uint32_t play_circle_cnt;
	uint32_t play_delay_cnt;
	uint32_t int_flag;
	uint32_t need_wait;
	uint16_t write_pos;
	uint16_t play_pos;
	wait_event_t dac_tx_waitq;
	uint8_t *data;

};

struct AUDIO_ADC
{
	uint8_t type;
	volatile uint8_t int_flag;
	volatile uint8_t pending_flag;
	unsigned int samplerate;
	struct adpcm_state stat;
	uint32_t recv_pos;
	uint32_t len;
	uint8_t *data;
	uint8_t *block_data;
};

struct MP3_HEADER
{
	unsigned int sync: 11; //ͬ����Ϣ
	unsigned int version: 2; //�汾
	unsigned int layer: 2; //��
	unsigned int protection: 1; // CRCУ��
	unsigned int bitrate_index: 4; //λ��
	unsigned int sampling_frequency: 2; //����Ƶ��
	unsigned int padding: 1; //֡������
unsigned int private:
	1; //������
	unsigned int mode: 2; //����ģʽ
	unsigned int mode_extension: 2; //����ģʽ
	unsigned int copyright: 1; // ��Ȩ
	unsigned int original: 1; //ԭ���־
	unsigned int emphasis: 2; //ǿ��ģʽ
};

#define AUDIO_RECORD_ROOM_SIZE	128*1024
#define AUDIO_RECORD_BASE_ADDR	(1024*1024 - AUDIO_RECORD_ROOM_SIZE)          //¼��������flash�����32k�ռ�
struct AUDIO_CFG
{
	uint8_t audio_dev_open;
	uint8_t volume; // 0 -100
	BOOL play_mp3, playing_mp3, play_next, pause, auto_resume;
	timer_t mp3_delay_timer;
	mutex_t push_mutex;
	struct AUDIO_DEC_INPUT *dec_input;
	struct AUDIO_DEC_OUTPUT *dec_output;
	struct WAVE_FORMAT_HEAD *pcm;
	struct AUDIO_ADC *adc;
    #if BAIDU_AI_VOICE_ENABLE
       msg_q_t ai_queue;
    #endif
	BOOL recording, recording_to_flash;
	timer_t recording_to_flash_timer;
	uint32_t recording_to_flash_addr;

	SpeexBits enc_bits, dec_bits;/* Holds bits so they can be read and written by the Speex routines */
	void *enc_state, *dec_state;/* Holds the states of the encoder & the decoder */
	int quality, complexity, vbr, enh;/* SPEEX PARAMETERS2?��y, MUST REMAINED UNCHANGED ����3?2?��?*/
	int enc_frame_size, dec_frame_size;

};

extern struct AUDIO_CFG audio_cfg;

void push_to_play_buff(uint8_t *val, int size);
void audio_dev_init(void);
int audio_dev_open(void);
int audio_dev_close(void);
int handle_audio_stream(unsigned char *buff, int size);
void switch_audio_mode(void);
void open_audio(void);
int iot_speex_encode(uint16_t *in_buff, int len/*short unit*/, uint8_t *out_buff, int out_buff_size/*byte unit*/);
int iot_speex_decode(uint8_t *in_buff, int len/*byte unit*/, uint16_t *out_buff, int out_buff_size/*byte unit*/);
void playback_record_audio(void);
void audio_record_to_flash(void);
#endif
