#include <socket_app.h>
#include <stm32f10x.h>

#include <app_def.h>

extern FLASH_DATA read_data;
extern char *r_p;

static void socket_thread_entry(void * parameter);
void socket_thread_init(void);

/* 消息队列控制块 */
struct rt_messagequeue socket_door_mq;
struct rt_messagequeue socket_flash_config_mq;
struct rt_messagequeue socket_read_config_mq;
/* 消息队列内存池 */
//static char socket_msg_pool[1024];
//static char buf[32];
static void udpserv(void );//* parameter);
//static int port = 32100;

extern struct rt_event NRF24L01_event;
static void socket_thread_entry(void * parameter)
{
	rt_err_t result;
//	result = rt_mq_init(&socket_mq,"socket_mq",
//							&socket_msg_pool[0],
//							sizeof(buf),
//							//sizeof(UDP_SEND_A),
//							sizeof(socket_msg_pool),
//							RT_IPC_FLAG_FIFO);
//	if(result != RT_EOK)
//	{
//		rt_kprintf("init message queue failed.\n");
//	}
	while(1)
	{
		udpserv();
	}
}

void socket_thread_init(void)
{
	rt_thread_t socket_thread;
	socket_thread = rt_thread_create("socket_thread",socket_thread_entry,
																		RT_NULL,512,22,20);
	if(socket_thread != RT_NULL)
	{
		rt_thread_startup(socket_thread);
	}
}

static void udpserv(void )//* parameter)
{
	int sock;
	int bytes_read;
	char *recv_data;
	char i,check;
	rt_uint32_t addr_len;
	rt_err_t result;
	struct sockaddr_in server_addr,client_addr;
	
	recv_data = rt_malloc(1024);
	if(recv_data == RT_NULL)
	{
		rt_kprintf("No memory\n");
		return;
	}
	
	if((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		rt_kprintf("Socket error\n");
		rt_free(recv_data);
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
		rt_free(recv_data);
		return;
	}
	addr_len = sizeof(struct sockaddr);
	rt_kprintf("UDPServer Waiting for client on port 3200...\n");
	
	while(1)
	{
	L1:	bytes_read = recvfrom(sock,recv_data,1024,0,
							(struct sockaddr *)&client_addr,&addr_len);
		recv_data[bytes_read] = '\0';
		
		rt_kprintf("\n(%s,%d) said : ",inet_ntoa(client_addr.sin_addr),
										ntohs(client_addr.sin_port));
		rt_kprintf("%s",recv_data);

		check = 0;
		for(i=0;i<bytes_read-1;i++)
			check += recv_data[i];

		rt_kprintf("%s	%d	%d\n",__FILE__,__LINE__,bytes_read);

		if((recv_data[8] >= 'C')&&(recv_data[8] <= 'I')&&(check == recv_data[bytes_read-1]))
			//EXTI->IMR &= ~0x02;		   it seems that IRQ is not masked.
			;
		else
			goto L1;
		rt_kprintf("%s	%d\n",__FILE__,__LINE__);
		switch(recv_data[8])
		{
			case 'I':									  //door_app
			result = rt_mq_send(&socket_door_mq,&recv_data[0],32);//sizeof(send));
			if(result == -RT_EFULL)
				rt_kprintf("%s	error\n",__FILE__);//break;							//若队列长度不够，则会跳出循环
			break;
			case 'D':									  //read_config_app
			case 'G':
			result = rt_mq_send(&socket_read_config_mq,&recv_data[0],32);//sizeof(send));
			if(result == -RT_EFULL)
				rt_kprintf("%s	error\n",__FILE__);//break;							//若队列长度不够，则会跳出循环
			break;
			case 'C':									  //flash_config_app
			case 'E':
			case 'F':
			result = rt_mq_send(&socket_flash_config_mq,&recv_data[0],32);//sizeof(send));
			if(result == -RT_EFULL)
				rt_kprintf("%s	error\n",__FILE__);//break;							//若队列长度不够，则会跳出循环
			break;
			default:
			break;
		}
			
		if(strcmp(recv_data,"exit") == 0)
		{
			lwip_close(sock);
			rt_free(recv_data);
			break;
		}
	}
	return;
}
