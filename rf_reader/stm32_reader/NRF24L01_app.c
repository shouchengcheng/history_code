#include "NRF24L01_app.h"
#include "led.h"

#include "app_def.h"

/*
NRF24L01_event: 1 nRF24L01(recv)	NRF24L01_app.c
				2 door(recv)		door_app.c
				3 config(recv)		flash_config.c
				4 read(recv)		read_config.c

*/

static UDP_SEND_A send;

static void NRF24L01_thread_entry(void * parameter);
void NRF24L01_thread_init(void);

struct rt_event NRF24L01_event;

extern struct rt_messagequeue packet_mq;

static void NRF24L01_thread_entry(void * parameter)
{
	char flag = 0;
	rt_err_t result;
	rt_uint32_t e;
	u8 tmp_buf[8],i = 0,SN = 0;
	u8 check = 0;
	
//	/* event init */
//	result = rt_event_init(&NRF24L01_event, "event", RT_IPC_FLAG_FIFO);
//	if (result != RT_EOK)
//	{
//		rt_kprintf("init event failed.\n");
//		return ;
//	}
	
	NRF24L01_Init();
	rt_hw_led_off(1);
	
	send.head.Fix[0] = 0x02;
	send.head.Fix[1] = 0x03;
	send.head.Fix[2] = 0x04;
	send.head.Fix[3] = 0x05;
	
	send.head.Length[0] = 0;
	send.head.Length[1] = 19;
   	send.head.Command = 'A';
	

	while(NRF24L01_Check())//检测不到24L01
  	{
		rt_hw_led_on(1);
		rt_thread_delay(100);
  	}
	RX_Mode();
	while(1)
	{
		SN++;
		send.head.SN = SN;
		send.body.Card_num = 1;
		send.body.Check_sum = 1;

	L1:	if (rt_event_recv(&NRF24L01_event, (1 << 1),
        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
        RT_WAITING_FOREVER, &e) == RT_EOK)
    	{
        	rt_kprintf("recv event \n");
    	}
		if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
		{
			rt_kprintf("message send\n");
			check = tmp_buf[0];
			for(i=1;i<8;i++)
			{
				check ^= tmp_buf[i];
			}
			if(check!=0x69)
				goto L1;
			for(i=0;i<7;i++)
				send.body.Card_ID[i] = tmp_buf[i+1];

			send.body.Check_sum = send.head.Fix[0]+send.head.Fix[1]+send.head.Fix[2]+send.head.Fix[3];
			send.body.Check_sum = send.body.Check_sum + send.head.Length[0]+send.head.Length[1];
			send.body.Check_sum = send.body.Check_sum + send.head.Read_ID[0]+send.head.Read_ID[1];
			send.body.Check_sum = send.body.Check_sum + send.head.Command;
			send.body.Check_sum = send.body.Check_sum + send.head.SN;
			send.body.Check_sum = send.body.Check_sum + send.body.Card_num;
			send.body.Check_sum = send.body.Check_sum + send.body.Card_ID[0]+send.body.Card_ID[1]+send.body.Card_ID[2]+
			send.body.Card_ID[3]+send.body.Card_ID[4]+send.body.Card_ID[5]+send.body.Card_ID[6];
			

			result = rt_mq_send(&packet_mq,&send.head.Fix[0],32);//sizeof(send));
			if(result == -RT_EFULL)
				break;							//若队列长度不够，则会跳出循环
		}
		if(flag)
		{
			rt_hw_led_on(1);
			flag = !flag;
		}
		else
		{
			rt_hw_led_off(1);
			flag = !flag;
		}
	}
	
	while(1)
	{
		rt_kprintf("the length of mq is not enough\n");
	}
}

void NRF24L01_thread_init(void)
{
	rt_thread_t NRF24L01_thread;
	
	NRF24L01_thread = rt_thread_create("NRF24L01_thread",
										NRF24L01_thread_entry,RT_NULL,
										256,23,20);
	if(NRF24L01_thread != RT_NULL)
	{
		rt_thread_startup(NRF24L01_thread);
	}
}
