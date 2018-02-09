#include <packet_app.h>

#include <lwip/netdb.h> 
#include <lwip/sockets.h> 

#include <app_def.h>
#include <stm32f10x.h>
UDP_SEND_A send;

extern FLASH_DATA read_data;
extern char *r_p;

static void packet_thread_entry(void * parameter);
void packet_thread_init(void);

/* 消息队列控制块 */
struct rt_messagequeue packet_mq;
/* 消息队列内存池 */
//static char packet_msg_pool[1024];
	int sock;

   	struct sockaddr_in server_addr;
	
										
static char buf[32];
static void packet_thread_entry(void * parameter)
{


   	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   	{
       rt_kprintf("Socket error\n");
       return;
   	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(read_data.flash_data_net.Sou_Port[0]*256+read_data.flash_data_net.Sou_Port[1]);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	rt_memset(&(server_addr.sin_zero),0,sizeof(server_addr.sin_zero));
	
	if(bind(sock,(struct sockaddr *)&server_addr,
								sizeof(struct sockaddr)) == -1)
	{
		rt_kprintf("Bind error\n");
		return;
	}

   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(read_data.flash_data_net.Des_Port[0]*256+read_data.flash_data_net.Des_Port[1]);
   server_addr.sin_addr = *(struct in_addr *)(read_data.flash_data_net.Des_IP_Addr);
   rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	
	while(1)
	{
		rt_memset(buf,0,sizeof(buf));
		if(rt_mq_recv(&packet_mq,buf,sizeof(buf),RT_WAITING_FOREVER) == RT_EOK)
		{
			rt_kprintf("recv msg from msg queue\n");

			sendto(sock, buf, buf[5], 0,
		 		(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		}
	}
}

void packet_thread_init(void)
{
	rt_thread_t packet_thread;
	packet_thread = rt_thread_create("packet_thread",
									packet_thread_entry,RT_NULL,
									512,20,20);
	if(packet_thread != RT_NULL)
	{
		rt_thread_startup(packet_thread);
	}
}

static char sbuf[]=
{
2,3,4,5,0,0x13,0,0,0x41,0,0,0,0,0,0,0,0,0,0x62
};
static void connect_thread_entry(void * parameter)
{
//	int sock;
//
//   	struct sockaddr_in server_addr;
//
//   	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
//   	{
//       rt_kprintf("Socket error\n");
//       return;
//   	}
//
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_port = htons(read_data.flash_data_net.Sou_Port[0]*256+read_data.flash_data_net.Sou_Port[1]);
//	server_addr.sin_addr.s_addr = INADDR_ANY;
//	rt_memset(&(server_addr.sin_zero),0,sizeof(server_addr.sin_zero));
//	
//	if(bind(sock,(struct sockaddr *)&server_addr,
//								sizeof(struct sockaddr)) == -1)
//	{
//		rt_kprintf("connect_Bind error\n");
//		return;
//	}
//
//   server_addr.sin_family = AF_INET;
//   server_addr.sin_port = htons(read_data.flash_data_net.Des_Port[0]*256+read_data.flash_data_net.Des_Port[1]);
//   server_addr.sin_addr = *(struct in_addr *)(read_data.flash_data_net.Des_IP_Addr);
//   rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	
	while(1)
	{		
		sendto(sock, sbuf, sbuf[5], 0,
	 		(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		rt_thread_delay(300);
	}
}

void connect_thread_init(void)
{
	rt_thread_t connect_thread;
	connect_thread = rt_thread_create("connect_thread",
									connect_thread_entry,RT_NULL,
									512,30,20);
	if(connect_thread != RT_NULL)
	{
		rt_thread_startup(connect_thread);
	}
}
