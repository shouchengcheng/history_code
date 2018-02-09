#include <door.h>
#include <rtthread.h>
#include <stm32f10x.h>

void rt_hw_door_init(void);
void rt_hw_door_on(rt_uint32_t door);
void rt_hw_door_off(rt_uint32_t door);

#define door_open_rcc                    RCC_APB2Periph_GPIOD
#define door_open_gpio                   GPIOD
#define door_open_pin                    (GPIO_Pin_2)

#define door_close_rcc                    RCC_APB2Periph_GPIOD
#define door_close_gpio                   GPIOD
#define door_close_pin                    (GPIO_Pin_3)

#define door_stop_rcc                    RCC_APB2Periph_GPIOD
#define door_stop_gpio                   GPIOD
#define door_stop_pin                    (GPIO_Pin_2)

void rt_hw_door_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(door_open_rcc,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
/*
	door_open
  */	
	GPIO_InitStructure.GPIO_Pin = door_open_pin;
	GPIO_Init(door_open_gpio,&GPIO_InitStructure);
/*
	door_close
	*/
	GPIO_InitStructure.GPIO_Pin = door_close_pin;
	GPIO_Init(door_close_gpio,&GPIO_InitStructure);
/*
	door_stop
	*/
	GPIO_InitStructure.GPIO_Pin = door_stop_pin;
	GPIO_Init(door_stop_gpio,&GPIO_InitStructure);
}

void rt_hw_door_on(rt_uint32_t door)
{
	switch(door)
	{
		case DOOR_OPEN:
			GPIO_ResetBits(door_open_gpio,door_open_pin);
			break;
		case DOOR_CLOSE:
			GPIO_ResetBits(door_close_gpio,door_close_pin);
			break;
		case DOOR_STOP:
			GPIO_ResetBits(door_stop_gpio,door_stop_pin);
			break;
		default:
			break;
	}
}

void rt_hw_door_off(rt_uint32_t door)
{
	switch(door)
	{
		case DOOR_OPEN:
			GPIO_SetBits(door_open_gpio,door_open_pin);
			break;
		case DOOR_CLOSE:
			GPIO_SetBits(door_close_gpio,door_close_pin);
			break;
		case DOOR_STOP:
			GPIO_SetBits(door_stop_gpio,door_stop_pin);
			break;
		default:
			break;
	}
}
