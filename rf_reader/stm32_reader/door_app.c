#include <door_app.h>
#include <door.h>

static void door_thread_entry(void * parameter);
void door_thread_init(void);


extern struct rt_messagequeue socket_door_mq;
extern struct rt_messagequeue packet_mq;
static char buf[32];

static void door_thread_entry(void * parameter)
{
	char i = 0;
	rt_err_t result;
	while(1)
	{
		rt_memset(buf,0,sizeof(buf));
		if(rt_mq_recv(&socket_door_mq,buf,sizeof(buf),RT_WAITING_FOREVER) == RT_EOK)
		{
			rt_kprintf("%s	%d\n",__FILE__,__LINE__);
		}
		
		if(buf[10])
		rt_hw_door_on(DOOR_OPEN);
		else
		rt_hw_door_off(DOOR_OPEN);
		if(buf[11])
		rt_hw_door_on(DOOR_CLOSE);
		else
		rt_hw_door_off(DOOR_CLOSE);
		if(buf[12])
		rt_hw_door_on(DOOR_STOP);
		else
		rt_hw_door_off(DOOR_STOP);
		buf[5] = 0x0d;
		buf[10] = 'O';
		buf[11] = 'K';
		buf[12] = 0;
		for(i=0;i<12;i++)
			buf[12] += buf[i];	
		result = rt_mq_send(&packet_mq,buf,sizeof(buf));//sizeof(send));
		if(result == -RT_EFULL)
			rt_kprintf("%s :error\n"__FILE__);
			//break;							//若队列长度不够，则会跳出循环
		//rt_thread_delay(100);
		//rt_hw_door_off(DOOR_OPEN);
		//rt_hw_door_off(DOOR_CLOSE);
		//rt_hw_door_off(DOOR_STOP);
		
	}
}

void door_thread_init(void)
{
	rt_thread_t door_thread;
	door_thread = rt_thread_create("door_thread",
					door_thread_entry,RT_NULL,
					2048,21,20);
	if(door_thread != RT_NULL)
	{
		rt_thread_startup(door_thread);
	}
}
