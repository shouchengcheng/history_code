
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
	char Des_Port[2];	//Ŀ��˿�														Ĭ�ϣ�32500
	char Sou_Port[2];	//���ض˿�
	char Des_IP_Addr[4];//Ŀ��IP
	char Sou_IP_Addr[4];//����IP															Ĭ�ϣ�32100
	char Channal_Mode;	//ͨѶģʽ														Ĭ�ϣ�2
	char Enable;		//����״̬														Ĭ�ϣ�1
}FLASH_DATA_NET;

typedef struct
{
	char Read_ID[2];	//�豸���	0~65535												Ĭ�ϣ�88
	char Send_Gain;		//��������	0:-18db	1:-12db	2:-6db	3:0db						Ĭ�ϣ�3
	char Recv_Gain;		//��������	0~31												Ĭ�ϣ�0x1f
	char Air_Speed;		//��������	0:250K	1:1M	2:2M								Ĭ�ϣ�0
	char Baud_Rate;		//������	0:4800	1:9600	2:19200	3:38400	4:57600	5:115200	Ĭ�ϣ�3
	char Buzzer;		//��������	0:off	1:on										Ĭ�ϣ�0
	char Sence_Place;	//���޵ظ�	0:off	1:on										Ĭ�ϣ�0
	char ID_Threshold;	//ID��ֵ	0~7													Ĭ�ϣ�0
	char Filter_Length;	//����ʱ��	0~255												Ĭ�ϣ�0
	char Time_Span;		//���ͼ��	0~255												Ĭ�ϣ�2
	char ID_Type;		//��ǩ����	0~255												Ĭ�ϣ�0x1e
	char CRC_Check;		//CRCУ��	0:off	1:on										Ĭ�ϣ�0
	char Hold;			//����		0~255												Ĭ�ϣ�0
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
