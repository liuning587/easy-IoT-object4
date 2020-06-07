#define DEBUG
//#define DEBUG_TASK
#include "drivers.h"
#include "app.h"
#include "api.h"
#include  "mq2.h"
#include  "debug.h"






#define CAPTURE_MODE_CIR		 			0	

/*
*����ģʽ�������ṩ���ֲ������ӣ�
*ѭ��ģʽ:ͼƬͨ��dma�Զ���ȡ��ѭ�����浽�������ڴ棬���ǿ�����֡�ж������ͼƬ������
*˫���巽ʽ:�������黺������������������ͼƬ������ģʽ�ܼ򵥣������˷Ѹ����ڴ棬���Ҳ���һ��ͼƬ��
*��Ҫ��ͣ��������������DMA
*
*/
#define CAPTURE_MODE 			CAPTURE_MODE_CIR    //ʹ��ѭ��ģʽ



#define CAMERA_IMG_QUEUE_LEN	1

extern  u8 * p;
    

extern void  cemera_cofig(void);
extern void  rgb565_test(void);


int   camera_mode( int  CAMERA_MODE  );

struct CAMERA_CFG camera_cfg;
extern wait_event_t client_camera_event;

void img_send_thread(void *arg);
int img_send_thread_id;
                



int deinit_camera()
{
	DCMI_Cmd(DISABLE);

	DMA_Cmd(DMA2_Stream1, DISABLE);

	DCMI_DeInit();

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, DISABLE);
	
	return 0;
}


extern void web_server_init(void);
int open_camera()
{ 
	cemera_cofig();
    web_server_init();
	return 0;
}


void DMA2_Stream1_IRQHandler() 
{
	enter_interrupt();

	if(DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_HTIF1))
	{
		DMA_ClearFlag(DMA2_Stream1,  DMA_FLAG_HTIF1);
		
	}

	if(DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_TCIF1))
	{
         printf("\r\n ****����DMA�����ж�2  *****\r\n");
		DMA_ClearFlag(DMA2_Stream1,  DMA_FLAG_TCIF1);
	}

	if(DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_TEIF1))
	{
		p_err("dcmi dma err");
		DMA_ClearFlag(DMA2_Stream1,  DMA_FLAG_TEIF1);
	}

 	exit_interrupt(0);
}





extern int mqtt_connected;

uint8_t *web_data = NULL;
mutex_t web_sig = NULL;





void img_send_thread(void *arg)
{

 
    while(1)
    {   
           p_dbg_enter_task;
           
   
        p_dbg_exit_task;
    }
}



