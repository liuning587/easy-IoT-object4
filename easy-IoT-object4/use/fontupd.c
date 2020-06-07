#include "fontupd.h"  
#include "drivers.h"    
#include "string.h"
#include "sys_misc.h"



//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)
#define FONTSECSIZE	 	791
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	1024*1024*12 					//Explorer STM32F4�Ǵ�1M��ַ�Ժ�ʼ����ֿ�
														//ǰ��4M��fatfsռ����.
														//4M�Ժ����3���ֿ�+UNIGBK.BIN,�ܴ�С11.59M,���ֿ�ռ����,���ܶ�!
														//15.59M�Ժ�,�û���������ʹ��.����������100K�ֽڱȽϺ�.
														
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;


u8 font_init(void)
{		
	u8 t=0;
	m25p80_init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		m25p80_read(FONTINFOADDR,sizeof(ftinfo),(u8*)&ftinfo);//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}


































