#include "stc11f60ex.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;
static BYTE UART_TIME;

static BYTE rev[13];

#define UART_START_TIME 0
#define UART_CONFIG_TIME 1
#define UART_OFF 3

/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE 0
#define CMD_READ 1
#define CMD_PROGRAM 2
#define CMD_ERASE 3

#define TIME 65535-356
static BYTE flag = 0,d1 = 0,d2 = 0,d3 = 0,d4 = 0,d5 = 0,d6 = 0;
static int count_t0 = 0;
/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
#define ENABLE_IAP 0x83

/*Start address for STC11L60XE series EEPROM*/
#define IAP_ADDRESS 0x0000

static xdata BYTE ID[][5] = {{ 0x08,0x00,0x00,0x01,0x04 },{ 0x08,0x02,0x00,0x00,0x08 }
,{ 0x08,0x02,0x00,0x00,0x08 },{ 0x08,0x02,0x00,0x00,0x08 }
,{ 0x08,0x02,0x00,0x00,0x08 },{ 0x08,0x02,0x00,0x00,0x08 }};
//4,4,4,4,2

static data BYTE carrier[][11] = {{ 0x1f,0xfe,0xaa,0xaa,0x59,0xa5,0x00,0x00,0x00,0x00,0x0f },{ 0x1f,0xfe,0xaa,0xaa,0x59,0xa5,0x00,0x00,0x00,0x00,0x00 }
,{ 0x1f,0xfe,0xaa,0xaa,0x59,0xa5,0x00,0x00,0x00,0x00,0x00 },{ 0x1f,0xfe,0xaa,0xaa,0x59,0xa5,0x00,0x00,0x00,0x00,0x00 }
,{ 0x1f,0xfe,0xaa,0xaa,0x59,0xa5,0x00,0x00,0x00,0x00,0x00 },{ 0x1f,0xfe,0xaa,0xaa,0x59,0xa5,0x00,0x00,0x00,0x00,0x00 }};

void IapIdle(void);
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr,BYTE dat);
void IapEraseSector(WORD addr);

void Timer0Init(void)		//10毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xF0;		//设置定时初值
	TH0 = 0xD8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	ET0 = 1;
	TR0 = 1;		//定时器0开始计时
}

void Init(void)
{
	CLK_DIV = 0x00;
/*************************uart**************************/
	AUXR = 0x97; //TA0 1T TA1 12T; use BRTR 1T to control UART;disable RAM

/*************************125KHz timer 0**************************/
	TMOD = 0x02;
	TH0 = (256-48);
	WAKE_CLKO = 0x01;
	//ET0 = 1;
	ET0 = 0;
/**************************timer 1********************************/
	TMOD |= 0x10;
	TL1 = TIME;
	TH1 = TIME>>8;
	ET1 = 1;
/*****************************************************************/
	EA = 1;	//打开全局中断
}
void timer0_isr() interrupt 1
{
	//P3^=0x10;
	//P2^=d2;
	TL0 = 0xF0;		//设置定时初值
	TH0 = 0xD8;		//设置定时初值
	if(++count_t0>=500)
	{
		TR0 = 0;
		UART_TIME = UART_OFF;
		count_t0 = 0;
		P0 |= 0x30;
	}else if((count_t0%50) == 0)
	{
		P0 ^= 0x30;
	}
	
}

void delay_us(int i)
{
	for(;i>0;)
	{
		i--;
	}
}

void delay_ms(int i)
{
	for(;i>0;)
	{
		i--;
		delay_us(1000);
	}
}
void uart_init(void)
{
	AUXR|=0x40;
	SCON=0X50;
	TMOD &= 0x0f;
	TMOD|=0X20;
	PCON=0x00;
	TH1=0Xd9;
	TL1=0Xd9;
	TR1=1;
	ES=1;
	EA=1;
	
}
void Translate(void)
{
	unsigned char i,j;
	for(j = 0;j < 6;j++)
	{
		for(i = 0;i < 5;i++)
		{
			  ID[j][i] = IapReadByte(IAP_ADDRESS+i+j*10);
		}
	}
}

void main(void)
{
	int i,j,n,count=0;	
	UART_TIME=UART_CONFIG_TIME;
	//UART_TIME = UART_OFF;
	Translate();
	Timer0Init();
L1:	uart_init();
	//P0 |= 0x30;
	count_t0 = 0;
	//Timer0Init();
	while(UART_TIME==UART_CONFIG_TIME)
	{
		if((rev[0]=='^')&&(rev[8]=='$'))
		{
			TR0 = 0;
			if((rev[1]=='W')||(rev[1]=='w'))
			{
				j=rev[2]-'1';
				if((j>=0)&&(j<6))
				{
					for(i=0;i<5;i++)
					{
						if((rev[3+i]>='a')&&(rev[3+i]<='f'))
						{
							ID[j][i] = rev[3+i] - 'a' + 10;
						}else if((rev[3+i]>='0')&&(rev[3+i]<='9'))
						{
							ID[j][i] = rev[3+i] - '0';
						}else if((rev[3+i]>='A')&&(rev[3+i]<='F'))
						{
							ID[j][i] = rev[3+i] - 'A' + 10;
						}
					}
					IapEraseSector(IAP_ADDRESS);
					for(j=0;j<6;j++)
					{
						for(i=0;i<5;i++)
						{
							IapProgramByte(IAP_ADDRESS+j*10+i,ID[j][i]);
						}
					}
				}		
			}else if((rev[1]=='R')||(rev[1]=='r'))
			{
				j=rev[2]-'1';
				
				if((j>=0)&&(j<6))
				{
					for(i=0;i<5;i++)
					{
						TI=0;
						if(ID[j][i]>=0x0a)
						{
							SBUF=ID[j][i]+'a'-10;
							//SBUF=IapReadByte(IAP_ADDRESS+i+j*10)+'0';
						}else
						{
							SBUF=ID[j][i]+'0';
							//SBUF=IapReadByte(IAP_ADDRESS+i+j*10)+'a'-10;
						}
						while(!TI);		
					}
					SBUF='$';
					//while(!TI);
				}
			}
			for(i=0;i<9;i++)
			{
				rev[i]=' ';
			}
			P0 ^= 0xf0;
			goto L1;
		}	
	}
	UART_TIME = UART_OFF;
	ES = 0;
	P0 |= 0x80;

	
	for(n = 0;n < 6;n++)
	{
		for(i = 0;i < 5;i++)
		{
			carrier[n][i+6] = 0x00;
			for(j = 0;j < 4;j++)
			{
				carrier[n][i+6] <<= 2;
				if(ID[n][i]&0x08)
				{
					carrier[n][i+6] += 0x02;
				}else
				{
					carrier[n][i+6] += 0x01;
				}
				ID[n][i] <<= 1;
			}
		}
	}
	Init();	
	while(1)
	{
		TL1 = TIME;
		TH1 = TIME>>8;
		TR0 = 1;
		P0 |= 0x10;
		flag = 0;
		TR1 = 1;
		while(flag == 0);
		P0 &= ~0x10;
		delay_ms(60);
	}
	
}

void timer1_isr() interrupt 3
{
	static unsigned char i = 8;
	static unsigned char dat1 = 0x00,dat2 = 0x00,dat3 = 0x00,dat4 = 0x00,dat5 = 0x00,dat6 = 0x00;
	static unsigned char num = 0;

	TL1 = TIME;
	TH1 = TIME>>8;
	
	if(i == 8)
	{
		if(num == 11)
		{
			num = 0;
			TR1 = 0;
			flag = 1;
			P2 &= ~0x01;
			P2 &= ~0X02;
		}
		i = 0;
		dat1 = carrier[0][num];
		dat2 = carrier[1][num];
		dat3 = carrier[2][num];
		dat4 = carrier[3][num];
		dat5 = carrier[4][num];
		dat6 = carrier[5][num];
		num += 1;
	}
	if(flag == 0){
	
			if(dat1&0x80)
			{
				P2 |= 0x01;
			}else
			{
				P2 &= ~0x01;
			}
			if(dat2&0x80)
			{
				P2 |= 0x02;
			}else
			{
				P2 &= ~0x02;
			}
			if(dat3&0x80)
			{
				P2 |= 0x04;
			}else
			{
				P2 &= ~0x04;
			}
			if(dat4&0x80)
			{
				P2 |= 0x08;
			}else
			{
				P2 &= ~0x08;
			}
			if(dat5&0x80)
			{
				P2 |= 0x10;
			}else
			{
				P2 &= ~0x10;
			}
			if(dat6&0x80)
			{
				P2 |= 0x20;
			}else
			{
				P2 &= ~0x20;
			}
	 
		if(num == 11)
		{
			if(i == 4)
			{
				i = 7;
			}
		}
		dat1 <<= 1;
		dat2 <<= 1;
		dat3 <<= 1;
		dat4 <<= 1;
		dat5 <<= 1;
		dat6 <<= 1;
		i += 1;
		//P2 = (d1 + d2 + d3 + d4 + d5 + d6);
	}
}

void Uart_Isr() interrupt 4
{
	static char i=0;
	if(RI)
	{
		RI=0;
		switch(UART_TIME)
		{
			case UART_START_TIME:
				UART_TIME = UART_CONFIG_TIME;
			break;
			case UART_CONFIG_TIME:
				P0 ^= 0x80;
				rev[i]=SBUF;
				if(rev[0]!='^')
				{
					i=0;
				}else
				{
					if(++i==9)
					{	
						i = 0;
						ES = 0;
					}
				}
			break;
		}
	}
}

/*Disable ISP/IAP/EEPROM function Make MCU in a safe state*/
void IapIdle(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0;
}

/*Read one byte from ISP/IAP/EEPROM area
Input:addr(ISP/IAP/EEPROM address)
Output:Flash data*/
BYTE IapReadByte(WORD addr)
{
	BYTE dat;

	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_READ;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr>>8;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();

	dat = IAP_DATA;
	IapIdle();

	return dat;
}

/*Program one byte to ISP/IAP/EEPROM area
Input:addr (ISP/IAP/EEPROM address)
dat (ISP/IAP/EEPROM data)
Output:-*/
void IapProgramByte(WORD addr,BYTE dat)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_PROGRAM;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr>>8;
	IAP_DATA = dat;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();

	IapIdle();
}

/*Erase one sector area
Input:addr(ISP/IAP/EEPROM address)
Output:-*/
void IapEraseSector(WORD addr)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_ERASE;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr>>8;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();

	IapIdle();
}
