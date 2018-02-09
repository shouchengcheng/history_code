#include <read_config_app.h>

#include <app_def.h>

extern FLASH_DATA read_data;
extern char *r_p;
	   
static void read_config_thread_entry(void * parameter);
void read_config_thread_init(void);

extern struct rt_messagequeue socket_read_config_mq;
extern struct rt_messagequeue packet_mq;
static char buf[32];

static void read_config_thread_entry(void * parameter)
{
	char i;
	char *p;
	rt_err_t result;
	while(1)
	{
		rt_memset(buf,0,sizeof(buf));
		if(rt_mq_recv(&socket_read_config_mq,buf,sizeof(buf),RT_WAITING_FOREVER) == RT_EOK)
		{
			rt_kprintf("%s	%d\n",__FILE__,__LINE__);
		}

		switch(buf[8])
		{
			case 'D':
			r_p = &read_data.flash_data_port.Read_ID[0];
			p = &buf[10];
			for(i=0;i<sizeof(FLASH_DATA_PORT);i++)
			{
				*p = *r_p;
				p++;
				r_p++;
			}
			buf[5] = 10+1+sizeof(FLASH_DATA_PORT);
			//buf[10] = 'O';
			//buf[11] = 'K';
			buf[10+sizeof(FLASH_DATA_PORT)] = 0;
			for(i=0;i<10+sizeof(FLASH_DATA_PORT);i++)
				buf[10+sizeof(FLASH_DATA_PORT)] += buf[i];
			break;
			case 'G':
			if(buf[10]==0x41)
			{
				buf[5] = 0x1e;
				r_p = &read_data.flash_data_net.Sou_IP_Addr[0];
				p = &buf[11];
				for(i=0;i<4;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				r_p = &read_data.flash_data_net.Mask[0];
				p = &buf[15];
				for(i=0;i<4;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				r_p = &read_data.flash_data_net.GetWall[0];
				p = &buf[19];
				for(i=0;i<4;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				r_p = &read_data.Mac[0];
				p = &buf[23];
				for(i=0;i<6;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				
				buf[29] = 0;
				for(i=0;i<29;i++)
					buf[29] += buf[i];
			}else if(buf[10]==0x42)
			{
				buf[5] = 0x16;
				r_p = &read_data.flash_data_net.Des_IP_Addr[0];
				p = &buf[11];
				for(i=0;i<4;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				r_p = &read_data.flash_data_net.Des_Port[0];
				p = &buf[15];
				for(i=0;i<2;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				r_p = &read_data.flash_data_net.Sou_Port[0];
				p = &buf[17];
				for(i=0;i<2;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				r_p = &read_data.flash_data_net.Channal_Mode;
				p = &buf[19];
				for(i=0;i<2;i++)
				{
					*p = *r_p;
					p++;
					r_p++;
				}
				
				buf[21] = 0;
				for(i=0;i<21;i++)
					buf[21] += buf[i];
			}
			break;
			default:
			break;
		}		

			
		result = rt_mq_send(&packet_mq,buf,sizeof(buf));//sizeof(send));
		if(result == -RT_EFULL)
			rt_kprintf("%s :error\n"__FILE__);
	}
}

void read_config_thread_init(void)
{
	rt_thread_t read_config_thread;
	read_config_thread = rt_thread_create("read_config_thread",
												read_config_thread_entry,RT_NULL,
												512,21,20);
	if(read_config_thread != RT_NULL)
	{
		rt_thread_startup(read_config_thread);
	}
}
