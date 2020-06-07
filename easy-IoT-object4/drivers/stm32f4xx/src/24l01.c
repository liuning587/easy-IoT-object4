/*
  ���Է���
	1. ��Ҫ���鿪����
	2. ���շ���������D,ע���д
	3. ���ͷ���������C,ע���д
	4. ���ͷ���������B,��������
*/

#define DEBUG
#include "drivers.h"
#include "bsp.h"

#include "debug.h"

const uint8_t NRF2401_TX_ADDRESS[NRF2401_TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const uint8_t NRF2401_RX_ADDRESS[NRF2401_RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ

bool nrf_ok = 0;
wait_event_t nrf_event;
//ioģ��SPI,ʱ�Ӵ�Լ3.5MHZ
static uint8_t io_spi_trans_byte(uint8_t byte)
{
	uint8_t i; 
	uint8_t Temp=0x00;
	unsigned char SDI; 
	for (i = 0; i < 8; i++)
	{
		GPIO_CLR(NRF_2401_CLK_GROUP, NRF_2401_CLK_PIN);
       
		if (byte&0x80)      
		{
			GPIO_SET(NRF_2401_MOSI_GROUP, NRF_2401_MOSI_PIN);
		}
		else
		{
			GPIO_CLR(NRF_2401_MOSI_GROUP, NRF_2401_MOSI_PIN);
		}
		byte <<= 1;  
         	GPIO_SET(NRF_2401_CLK_GROUP, NRF_2401_CLK_PIN);
	 
         	SDI = GPIO_STAT(NRF_2401_MISO_GROUP, NRF_2401_MISO_PIN);
         	Temp<<=1;
        
        	if(SDI)
         	{
              	Temp++;
         	}
         	GPIO_CLR(NRF_2401_CLK_GROUP, NRF_2401_CLK_PIN);
 	}
 
	return Temp; //���ض���miso�����ֵ     
   
}

uint8_t nrf_recv_buff[32];
void nrf2401_recv_thread(void *arg)
{
	uint8_t sta;
	
	while(1)
	{
		wait_event_timeout(nrf_event, 1000);

		sta=NRF24L01_Read_Reg(NRF2401_STATUS);
		NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_STATUS, sta);
		//p_dbg("sta:%x", sta);

		if(sta & NRF2401_TX_FIFO_FULL)
		{
			NRF24L01_Write_Reg(NRF2401_FLUSH_TX,0xff);
			p_dbg("NRF2401_TX_FIFO_FULL");
		}

		if(sta &  NRF2401_MAX_TX )
		{
			NRF24L01_Write_Reg(NRF2401_FLUSH_TX,0xff);
			//RX_Mode();
			p_dbg("NRF2401_MAX_TX");
		}

		if(sta &  NRF2401_TX_OK )
		{
			p_dbg("NRF2401_TX_OK");
			//RX_Mode();
		}

		if(sta &  NRF2401_RX_OK  )
		{
			p_dbg("NRF2401_RX_OK");
			NRF24L01_Read_Buf(NRF2401_RD_RX_PLOAD,nrf_recv_buff,NRF2401_RX_PLOAD_WIDTH);//��ȡ����
			NRF24L01_Write_Reg(NRF2401_FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
			dump_hex("data", nrf_recv_buff, 32);
		}

	}
}
							    
//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
  	NVIC_InitTypeDef   NVIC_InitStructure;
	int ret;
	p_dbg_enter;
	gpio_cfg((uint32_t)NRF_2401_CLK_GROUP, NRF_2401_CLK_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)NRF_2401_MISO_GROUP, NRF_2401_MISO_PIN, GPIO_Mode_IPU);
	gpio_cfg((uint32_t)NRF_2401_MOSI_GROUP, NRF_2401_MOSI_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)NRF_2401_CS_GROUP, NRF_2401_CS_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)NRF_2401_CE_GROUP, NRF_2401_CE_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)NRF_2401_IRQ_GROUP, NRF_2401_IRQ_PIN, GPIO_Mode_IPU);

	NRF_2401_CE_LOW;	//ʹ��24L01
	NRF_2401_CS_HIGH;	//SPIƬѡȡ��		

	ret = NRF24L01_Check();
	if(ret){
		p_dbg("2401 err");
		return;
	}

	nrf_event = init_event();

	SYSCFG_EXTILineConfig(NRF_2401_SOURCE_GROUP, NRF_2401_SOURCE_PIN);
	EXTI_InitStructure.EXTI_Line = NRF_2401_LINE;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	//�������̽ͷĬ���Ǹߵ�ƽ
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI15_10_IRQn_Priority;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//RX_Mode();

	thread_create(nrf2401_recv_thread, 0, TASK_NRF_PRIO, 0, TASK_NRF_STACK_SIZE, "nrf2401_recv_thread");

	nrf_ok = 1;
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	
	NRF24L01_Write_Buf(NRF2401_WRITE_REG+NRF2401_TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(NRF2401_TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
   	NRF_2401_CS_LOW;                 //ʹ��SPI����
  	status =io_spi_trans_byte(reg);//���ͼĴ����� 
  	io_spi_trans_byte(value);      //д��Ĵ�����ֵ
  	NRF_2401_CS_HIGH;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
 	NRF_2401_CS_LOW;          //ʹ��SPI����		
  	io_spi_trans_byte(reg);   //���ͼĴ�����
  	reg_val=io_spi_trans_byte(0XFF);//��ȡ�Ĵ�������
  	NRF_2401_CS_HIGH;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,u8_ctr;	       
  	NRF_2401_CS_LOW;           //ʹ��SPI����
  	status=io_spi_trans_byte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=io_spi_trans_byte(0XFF);//��������
  	NRF_2401_CS_HIGH;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,u8_ctr;	    
 	NRF_2401_CS_LOW;          //ʹ��SPI����
  	status = io_spi_trans_byte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)io_spi_trans_byte(*pBuf++); //д������	 
  	NRF_2401_CS_HIGH;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	NRF_2401_CE_LOW;
  	NRF24L01_Write_Buf(NRF2401_WR_TX_PLOAD,txbuf,NRF2401_TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF_2401_CE_HIGH;//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(NRF2401_STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&NRF2401_MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(NRF2401_FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return NRF2401_MAX_TX; 
	}
	if(sta&NRF2401_TX_OK)//�������
	{
		return NRF2401_TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	sta=NRF24L01_Read_Reg(NRF2401_STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&NRF2401_RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(NRF2401_RD_RX_PLOAD,rxbuf,NRF2401_RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(NRF2401_FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}				

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(void)
{
	p_dbg_enter;
	NRF_2401_CE_LOW;	  
  	NRF24L01_Write_Buf(NRF2401_WRITE_REG+NRF2401_RX_ADDR_P0,(uint8_t*)NRF2401_RX_ADDRESS,NRF2401_RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_RF_CH,40);	     //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_RX_PW_P0,NRF2401_RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF_2401_CE_HIGH; //CEΪ��,�������ģʽ 
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(void)
{									
	p_dbg_enter;
	NRF_2401_CE_LOW;	    
  	NRF24L01_Write_Buf(NRF2401_WRITE_REG+NRF2401_TX_ADDR,(uint8_t*)NRF2401_TX_ADDRESS,NRF2401_TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(NRF2401_WRITE_REG+NRF2401_RX_ADDR_P0,(uint8_t*)NRF2401_RX_ADDRESS,NRF2401_RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_RF_CH,40);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF2401_WRITE_REG+NRF2401_CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_2401_CE_HIGH;//CEΪ��,10us����������
}		  

void nrf2401_tx(uint8_t *data, int len)
{
	p_dbg_enter;

	if(!nrf_ok)
		return;
	
	//TX_Mode();

	NRF_2401_CE_LOW;
  	NRF24L01_Write_Buf(NRF2401_WR_TX_PLOAD,data,NRF2401_TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF_2401_CE_HIGH;	//��������	   


	p_dbg_exit;
}

/*
*NRF�ж�
*
*/
void __EXTI11_IRQHandler(void)
{
	if(nrf_event)
	wake_up(nrf_event);
}


