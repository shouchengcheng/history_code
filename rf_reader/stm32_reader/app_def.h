
typedef struct 
{
	char Fix[4];
	char Length[2];
	char Read_ID[2];
	char Command;
	char SN;
}UDP_HEAD;

typedef struct
{
	char Card_num;
	char Card_ID[7];
	char Check_sum;
}UDP_A;

//typedef struct
//{
//	
//}UDP_B;
//
//typedef struct
//{
//	
//}UDP_C;
//
//typedef struct
//{
//	
//}UDP_D;
//
//typedef struct
//{
//	
//}UDP_E;
//
//typedef struct
//{
//	
//}UDP_F;

typedef struct 
{
	UDP_HEAD head;
	UDP_A body;
}UDP_SEND_A;

typedef struct
{ 
	char Mask[4];
	char GetWall[4];
	char Des_Port[2];	//目标端口														默认：32500
	char Sou_Port[2];	//本地端口
	char Des_IP_Addr[4];//目标IP
	char Sou_IP_Addr[4];//本地IP															默认：32100
	char Channal_Mode;	//通讯模式														默认：2
	char Enable;		//激活状态														默认：1
}FLASH_DATA_NET;

typedef struct
{
	char Read_ID[2];	//设备编号	0~65535												默认：88
	char Send_Gain;		//发送增益	0:-18db	1:-12db	2:-6db	3:0db						默认：3
	char Recv_Gain;		//接收增益	0~31												默认：0x1f
	char Air_Speed;		//空中速率	0:250K	1:1M	2:2M								默认：0
	char Baud_Rate;		//波特率	0:4800	1:9600	2:19200	3:38400	4:57600	5:115200	默认：3
	char Buzzer;		//蜂鸣开关	0:off	1:on										默认：0
	char Sence_Place;	//有无地感	0:off	1:on										默认：0
	char ID_Threshold;	//ID阀值	0~7													默认：0
	char Filter_Length;	//过滤时长	0~255												默认：0
	char Time_Span;		//发送间隔	0~255												默认：2
	char ID_Type;		//标签类型	0~255												默认：0x1e
	char CRC_Check;		//CRC校验	0:off	1:on										默认：0
	char Hold;			//保留		0~255												默认：0
}FLASH_DATA_PORT;

typedef struct
{
	char check_net;
	char check_port;
	char Mac[6];
	
	FLASH_DATA_NET flash_data_net;
	FLASH_DATA_PORT flash_data_port;
}FLASH_DATA;

#define APP_OK	0xaa

#define	APP_BASE_ADDR	0x0803f800

#define	APP_CHECK	0*2
#define	APP_MAC		2*2
#define	APP_MASK	8*2
#define	APP_GETWALL	12*2
#define	APP_DES_PORT	14*2
#define	APP_SOU_PORT	16*2
#define	APP_DES_IP	20*2
#define	APP_SOU_IP	24*2
#define	APP_CHANNAL	28*2
#define	APP_READ_ID	30*2
#define	APP_SEND_RECV_GAIN	32*2
#define	APP_AIR_BAUD_SPEED	34*2
#define	APP_BUZZER_SENCE_PLACE	36*2
#define	APP_ID_THRESHOLD_FILTER	38*2
#define	APP_TIME_SPAN_ID_TYPE	40*2
#define	APP_CRC_CHECK_HOLD	42*2

char read_addr(int addr);
