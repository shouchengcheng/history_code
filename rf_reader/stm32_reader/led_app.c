
#include "led_app.h"
#include "beep.h"
#include "stm32f10x.h"


static void led_thread_entry(void * parameter);
void led_thread_init(void);

//ALIGN(4)
//static rt_uint8_t led_stack[512];
//static struct rt_thread led_thread;
static void led_thread_entry(void * parameter)
{
	rt_hw_led_init();
	rt_hw_beep_init();/////////scc/////////////
	TIM_Cmd(TIM2,ENABLE);
	rt_thread_delay(50);
	TIM_Cmd(TIM2,DISABLE);
	
	while(1)
	{
		//rt_kprintf("%s	%d\n",__FILE__,__LINE__);
		
		rt_hw_led_on(0);
		rt_thread_delay(100);
		
		
		rt_hw_led_off(0);
		rt_thread_delay(100);
	}
}
void led_thread_init(void)
{
// 	rt_err_t result;
	rt_thread_t led_thread;
	led_thread = rt_thread_create("led",
						led_thread_entry,RT_NULL,
						256,24,5);
	if(led_thread != RT_NULL)
	{
		rt_thread_startup(led_thread);
	}
}
