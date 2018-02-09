#include <flash_config_app.h>
#include <stdint.h>
#include <stm32f10x_flash.h>
#include <app_def.h>

extern FLASH_DATA read_data;
extern char *r_p;

static void flash_config_thread_entry(void * parameter);
void flash_config_thread_init(void);

extern struct rt_messagequeue socket_flash_config_mq;
extern struct rt_messagequeue packet_mq;
static char buf[32];

static void flash_config_thread_entry(void * parameter)
{
	char i;
	char *p;
	unsigned short *p16;
	rt_err_t result;
	while(1)
	{
		rt_memset(buf,0,sizeof(buf));
		if(rt_mq_recv(&socket_flash_config_mq,buf,sizeof(buf),RT_WAITING_FOREVER) == RT_EOK)
		{
			rt_kprintf("%s	%d\n",__FILE__,__LINE__);
		}

		switch(buf[8])
		{
			case 'C':
			r_p = &read_data.flash_data_port.Read_ID[0];
			p = &buf[10];
			for(i=0;i<sizeof(FLASH_DATA_PORT);i++)
			{
				*r_p = *p;
				p++;
				r_p++;
			}
			
			p16 = (unsigned short *)(&(read_data.check_net));

			read_data.check_net = APP_OK;
			read_data.check_port = APP_OK;
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
			FLASH_ErasePage(APP_BASE_ADDR);
			for(i=0;i<sizeof(FLASH_DATA);i++)
			{
				rt_kprintf("%d	",read_addr(APP_BASE_ADDR+i));
				r_p++; 		
			}
			for(i=0;i<sizeof(FLASH_DATA);i=i+2)
			{
				FLASH_ProgramHalfWord(APP_BASE_ADDR+i,*(p16+i/2));	
			}
			FLASH_Lock();
			break;
			case 'E':
			read_data.flash_data_port.Send_Gain	= buf[10];
			read_data.flash_data_port.Recv_Gain	= buf[11];
			read_data.flash_data_port.Buzzer = buf[12];

			p16 = (unsigned short *)(&(read_data.check_net));

			read_data.check_net = APP_OK;
			read_data.check_port = APP_OK;
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
			FLASH_ErasePage(APP_BASE_ADDR);
			for(i=0;i<sizeof(FLASH_DATA);i++)
			{
				rt_kprintf("%d	",read_addr(APP_BASE_ADDR+i));
				r_p++; 		
			}
			for(i=0;i<sizeof(FLASH_DATA);i=i+2)
			{
				FLASH_ProgramHalfWord(APP_BASE_ADDR+i,*(p16+i/2));	
			}
			FLASH_Lock();
			break;
			case 'F':
			if(buf[10] == 0x41)
			{
				for(i=0;i<4;i++)
				{
					read_data.flash_data_net.Sou_IP_Addr[i] = buf[11+i];
				}
				for(i=0;i<4;i++)
				{
					read_data.flash_data_net.Mask[i] = buf[15+i];
				}
				for(i=0;i<4;i++)
				{
					read_data.flash_data_net.GetWall[i] = buf[19+i];
				}
			}else if(buf[10] == 0x42)
			{
				for(i=0;i<4;i++)
				{
					read_data.flash_data_net.Des_IP_Addr[i] = buf[11+i];
				}
				for(i=0;i<2;i++)
				{
					read_data.flash_data_net.Des_Port[i] = buf[15+i];
				}
				for(i=0;i<2;i++)
				{
					read_data.flash_data_net.Sou_Port[i] = buf[17+i];
				}
				read_data.flash_data_net.Channal_Mode = buf[19];
				read_data.flash_data_net.Enable = buf[20];

			}

			p16 = (unsigned short *)(&(read_data.check_net));

			read_data.check_net = APP_OK;
			read_data.check_port = APP_OK;
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
			FLASH_ErasePage(APP_BASE_ADDR);
			for(i=0;i<sizeof(FLASH_DATA);i++)
			{
				rt_kprintf("%d	",read_addr(APP_BASE_ADDR+i));
				r_p++; 		
			}
			for(i=0;i<sizeof(FLASH_DATA);i=i+2)
			{
				FLASH_ProgramHalfWord(APP_BASE_ADDR+i,*(p16+i/2));	
			}
			FLASH_Lock();
			break;
			default:
			break;
		}

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
	}
}

void flash_config_thread_init(void)
{
	rt_thread_t flash_config_thread;
	flash_config_thread = rt_thread_create("flash_config_thread",
												flash_config_thread_entry,RT_NULL,
												512,21,20);
	if(flash_config_thread != RT_NULL)
	{
		rt_thread_startup(flash_config_thread);
	}
}
