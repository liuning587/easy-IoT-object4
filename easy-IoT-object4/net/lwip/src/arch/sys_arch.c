#include "lwip/arch/sys_arch.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "api.h"
#include "app.h"

struct timeoutnode 
{
	struct sys_timeouts timeouts;
	uint8_t prio;
	struct timeoutnode *next;
};

struct timeoutnode nulltimeouts;

struct timeoutnode *timeoutslist;

const void * const pvNullPointer = 0;

//�������ƣ�sys_init
//�������ܣ�����û�����Ĳ���ϵͳģ����ʼ������
void sys_init(void)
{
    timeoutslist = &nulltimeouts;
    
    nulltimeouts.timeouts.next = NULL; 
    
    nulltimeouts.next = NULL;
}

u32_t sys_jiffies(void)
{
	return jiffies;
}

u32_t sys_now(void)
{
	return os_time_get();     //????
}

err_t sys_sem_new(sys_sem_t *sem,u8_t count)      //����һ���ź������źų�ʼ��ֵΪcount  ���Ϊ sem
{
	sem_init((sem_t *)sem, count);
	if(*sem == 0)
	{
		p_err("sys_sem_new err");
		return ERR_VAL;
	}
	return ERR_OK;
	
}

int sys_sem_valid(sys_sem_t *sem)
{
	if(*sem == 0)
		return 0;
	return 1;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = 0;
}
/*
;*****************************************************************************************************
;* �������� : sys_sem_free
;* ��    �� : ɾ��һ���ź�
;* �䡡	 �� : sem: �źž��
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_sem_free(sys_sem_t *sem)
{
	sem_destory((sem_t *)sem);
}


/*
;*****************************************************************************************************
;* �������� : sys_sem_signal
;* ��    �� : ����һ���ź�
;* �䡡	 �� : sem: �źž��
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_sem_signal(sys_sem_t *sem)
{
	sem_post(sem);
}


/*
;*****************************************************************************************************
;* �������� : sys_arch_sem_wait
;* ��    �� : �ȴ�һ���ź�
;* �䡡	 �� : sem: �źž��, timeout: �ȴ���ʱ��΢����
;*        
;* �䡡	 �� : �ȴ����õ�΢����
;*****************************************************************************************************
;*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	int ret = SYS_ARCH_TIMEOUT;
	u32_t wait_ms = tick_ms;

	ret = sem_wait((sys_sem_t *)sem, timeout);
	if(ret == 0)
		ret = tick_ms - wait_ms;
	else if(ret == 1)
		ret = SYS_ARCH_TIMEOUT;
	return ret;
}


/*
;*****************************************************************************************************
;* �������� : sys_mbox_new
;* ��    �� : ����һ������
;* �䡡	 �� : size: ��������(ʵ�ʲ�������)
;*        
;* �䡡	 �� : sys_mbox_t: ������
;*****************************************************************************************************
;*/


err_t sys_mbox_new(sys_mbox_t *pMbox, sys_sem_t *sem, int size)
{

	err_t ret = ERR_OK;
	int real_size = min(size,MAX_MSG_IN_LWIP_MBOX);
	
	*pMbox = (sys_mbox_t)mem_calloc(sizeof(LWIP_MBOX),1);
	
	if (*pMbox == NULL)
	{
		ret = 1;
		goto end;
	}
	msgget(&((*pMbox)->lwip_mbox_e), real_size);
	
	if ((*pMbox)->lwip_mbox_e == NULL)
	{
		mem_free(*pMbox);
		*pMbox = 0;
		ret = 2;
		goto end;
	}

	sem_init((sem_t *)sem, 0);
	if(*sem == 0)
	{
		ret = 3;
		p_err("sys_mbox_new OSSemCreate err\n");
	}
end:
	if(ret != ERR_OK)
		p_err("sys_mbox_new err:%d\n",ret);
	return ret;
}

int sys_mbox_valid(sys_mbox_t *pMbox)
{
	if(*pMbox == 0)
		return 0;
	return 1;
}

void sys_mbox_set_invalid(sys_mbox_t *pMbox)
{
	*pMbox = 0;
}
/*
;*****************************************************************************************************
;* �������� : sys_mbox_post
;* ��    �� : �����ʼ�������
;* �䡡	 �� : mbox: ������, msg: �ʼ�
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_mbox_post(sys_mbox_t *mbox, sys_sem_t *sem, void *msg)
{
	int ret;
	int retry_cnt = 0;
	assert(mbox && *mbox);
	if (msg == NULL)
		msg = (void*)&pvNullPointer;
again:
	ret = msgsnd((*mbox)->lwip_mbox_e, msg);
	if(ret == 1) 	//OS_ERR_Q_FULL
	{
		if(in_interrupt()){
			mem_free(msg);
			return;
		}
		sem_wait(sem, 100); //??5?tick,
		if(retry_cnt > 50)
		{
			p_err_fun;
			mem_free(msg);
			return;
		}
		goto again;
	}

}


/*
;*****************************************************************************************************
;* ???? : sys_mbox_trypost
;* ?    ? : ?????????
;* ? 	 ? : mbox: ????, msg: ??
;*        
;* ? 	 ? : ERROR: ERR_MEM | OK: ERR_OK
;*****************************************************************************************************
;*/

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t ret = ERR_OK;
	assert(mbox && *mbox);
	if (msg == NULL)
		msg = (void*)&pvNullPointer;

	if (msgsnd((*mbox)->lwip_mbox_e, msg) != 0)
	{
		ret = ERR_MEM;
		goto end;
	}
end:

	return ret;
}
/*
;*****************************************************************************************************
;* �������� : sys_arch_mbox_fetch
;* ��    �� : ������ȴ�һ���ʼ�
;* �䡡	 �� : mbox: ������, msg: �ʼ�, timeout: �ȴ���ʱ��΢����
;*        
;* �䡡	 �� : �ȴ����õ�΢����
;*****************************************************************************************************
;*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, sys_sem_t *sem, void **msg, u32_t timeout)
{
	int err;
	u32_t wait_ms = tick_ms;
	void *Data;
	int ret = 0;
	assert(mbox && *mbox);

	err = msgrcv((*mbox)->lwip_mbox_e, &Data, timeout);
	if (err == 0)		
	{
	 	if (Data == (void*)&pvNullPointer)
	  {
			*msg = NULL;
	  }
		else
		{
			*msg = Data;
		}
		ret = tick_ms - wait_ms;
	}else{
		ret = SYS_ARCH_TIMEOUT;
	}

	if(*msg && is_sem_empty(sem))
	{
		sem_post(sem);
	}
		
	return ret;
}


/*
;*****************************************************************************************************
;* ???? : sys_arch_mbox_tryfetch
;* ?    ? : ???????????
;* ? 	 ? : mbox: ????, msg: ??
;*        
;* ? 	 ? : ERROR: SYS_MBOX_EMPTY | OK: 0
;*****************************************************************************************************
;*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	int err;
	void *Data;
	int ret;
	
	assert(mbox && *mbox);

	//Data = OSQAccept((*mbox)->lwip_mbox_e, &err);
	err = msgrcv((*mbox)->lwip_mbox_e, &Data, 1);
	if (err == 0)			
	{
	        if (Data == (void*)&pvNullPointer)
	        {
	            *msg = NULL;
	        }
	        else
	        {
	            *msg = Data;
	        }
		ret = 0;
	}	
	else
	{
        	ret = SYS_MBOX_EMPTY;
    	}
	//if(ret != 0)
	//	p_err("sys_arch_mbox_tryfetch err:%d\n",ret);
	return ret;
}

	
/*
;*****************************************************************************************************
;* �������� : sys_mbox_free
;* ��    �� : ɾ��һ������
;* �䡡	 �� : mbox: ������
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_mbox_free(sys_mbox_t *mbox,  sys_sem_t *sem)
{
	assert(mbox && *mbox);
	
	msgfree((*mbox)->lwip_mbox_e);
	
	mem_free(*mbox);

	sem_destory(sem);
}

//lwip���񴴽�����
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio) 
{

	return thread_create(thread,
                        arg,
                        prio,
                        0,
                        stacksize,
                        (char*)name);
}	



/*
;*****************************************************************************************************
;*                            			End Of File
;*****************************************************************************************************
;*/


	 

