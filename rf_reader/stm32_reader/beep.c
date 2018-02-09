#include "beep.h"

#include <stm32f10x.h>


#define beep_rcc                    RCC_APB2Periph_GPIOB
#define beep_gpio                   GPIOB
#define beep_pin                    (GPIO_Pin_5)

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel	=	TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority	=	1;
	NVIC_InitStructure.NVIC_IRQChannelCmd	=	ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}


void rt_hw_beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_Config();	

	RCC_APB2PeriphClockCmd(beep_rcc,ENABLE);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin	=	beep_pin;
	GPIO_Init(beep_gpio,&GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period	=	(500-1);
	TIM_TimeBaseStructure.TIM_Prescaler	=	(72-1);
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//TIM_Cmd(TIM2,ENABLE);

}

void TIM2_IRQHandler(void)
{
	static char i=0;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		if(i==0)
		{
			GPIO_SetBits(beep_gpio, beep_pin);
			i=1;
		}
		else
		{
			GPIO_ResetBits(beep_gpio, beep_pin);
			i=0;
		}
	}
}