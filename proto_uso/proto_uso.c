#include "proto_uso.h"
#include "calibrate\calibrate.h"
#include <intrins.h>

extern bit SHOW_VOLTAGE;

sbit DE_RE=P3^5;
// sbit LED=P2^6;
sbit LED=P0^6;
//-----------------------------------------------------------------------------------
//volatile unsigned char xdata DEV_NAME[DEVICE_NAME_LENGTH_SYM] ="<<uUSO_2>>"; //��� ����������
//volatile unsigned char xdata NOTICE[DEVICE_DESC_MAX_LENGTH_SYM]="<-- GEOSPHERA_2011 -->";//���������� 	
//volatile unsigned char xdata VERSION[DEVICE_VER_LENGTH_SYM] ="\x30\x30\x30\x30\x31";	// ������ ��������� ���	�� ������ 5 ����

volatile unsigned char idata ADRESS_DEV=0x1;

//volatile unsigned char xdata dev_desc_len=20;//����� �������� ����������
//--------------------------------global variable------------------------------------
//volatile unsigned char idata	PROTO_STATE;//������� ���������
volatile unsigned char idata	RECIEVED=0;//�������
volatile unsigned char idata    recieve_count;//������� ��������� ������
volatile unsigned char idata	transf_count;//������� ������������ ������	   
volatile unsigned char idata	buf_len;//����� ������������� ������

//------------------------����� ������--------------------------------
//volatile unsigned char idata CRC_ERR;	//������ �rc
//volatile unsigned char idata COMMAND_ERR;//���������������� �������

//volatile unsigned char idata TIMEOUT;//������� 

volatile unsigned char idata  CUT_OUT_NULL;//����-�������� 0 ����� 0xD7
volatile unsigned char idata frame_len=0;//����� �����, ������� ����������� �� ������� ����� �����
//--------------------------------------------------------------------
volatile unsigned char xdata  RecieveBuf[MAX_LENGTH_REC_BUF]={0} ; //����� ����������� ������
volatile unsigned char xdata 			*TransferBuf;
//static unsigned char /*data*/ volatile  TransferBuf[MAX_LENGTH_TR_BUF] ; //����� ������������ ������
extern volatile struct SKD xdata skd ;

struct pt pt_proto;
//--------------------------------------------------------------------
//volatile unsigned char xdata  STATE_BYTE=0xC0;//���� ��������� ����������
//-----------------------------------------------------------------------------------
union //����������� ��� ��������������� char->long
{
	float result_float;
	unsigned char result_char[4];
}
sym_8_to_float;
//-----------------------------------------------------------------------------------

void UART_ISR(void) interrupt 4 //using 1
{	
  unsigned char idata symbol=0xFF;//�������� ������

	EA=0;	//������ ����������
	
	if(RI)
	{
		RI=0; 
//----------------------������������ ��������� ������ ����� �����-------------
		if(recieve_count>MAX_LENGTH_REC_BUF)	//���� ������� ������� �������
		{
//			PROTO_STATE=PROTO_RESTART;//������ ������� ���������
			PT_RESTART_OUT(pt_proto);  //������������� �������

			return;
		} 



		symbol=SBUF;
		
//--------------------------������ �����...�������� �� ����� �����--------
	    if(recieve_count<6)
		{
				switch(recieve_count)	 
				{
					case  0:   //������ ������ 0
					{
	 				 	 if(symbol!=0x00)
						 {
						 	//PROTO_STATE=PROTO_RESTART;
							PT_RESTART_OUT(pt_proto);
							EA=1;
							return;								 	
						 }	 
					}
					break;
	
					case 1:	 //������ ������ 0xD7
					{
						 if(symbol!=0xD7)
						 {
						 	//PROTO_STATE=PROTO_RESTART;
							PT_RESTART_OUT(pt_proto);
							EA=1;
							return;
						 }		 
					}
					break;
	
					case 2:	 //	������ ������ 0x29
					{
					 	 if(symbol!=0x29)
						 {
						 	//PROTO_STATE=PROTO_RESTART;
							PT_RESTART_OUT(pt_proto);
							EA=1;
							return;	
						 }	 
					}
					break;

					case 3:	//���� ����� �� ������, �� �������//NEW
					{
						if(symbol!=ADRESS_DEV)//���� ����� ������	  
						{
							//PROTO_STATE=PROTO_RESTART;		//����� ���� ����� �������
							PT_RESTART_OUT(pt_proto);		
						}
					}
					break;
	
					default:  //
					{					
					}
					break;
				}

			RecieveBuf[recieve_count]=symbol;//��������� �������� ������ � �����
			recieve_count++;//������� ������

			if(recieve_count==6)
			{
				frame_len=RecieveBuf[recieve_count-1]; //������� ���������� �����
			}
		}
//---------------------------------------------------------
		else  //������ ����� ����� ����� � ������� ���� ����� 0xD7
		{							
			switch(symbol)//��������, ��� 0x0 ,0xD7 ��� ������
			{
				case 0xD7:
				{
					CUT_OUT_NULL=1;	//
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;
						
				}
				break;

				case 0x0:
				{
					if(!CUT_OUT_NULL)  //���� �������� 0x0 �� ����, �� �� ��������
					{
						RecieveBuf[recieve_count]=symbol;
						recieve_count++;								
					}		
					else //����� � ����� �� ������, ������� ����
					{
						CUT_OUT_NULL=0;			
					}	
				}
				break;

				default:  //������ ������ � �����
				{			
					CUT_OUT_NULL=0;
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;
				}
				break;
			}

			if(recieve_count>=frame_len+6)//���� �������  ��� �����
			{
				//PROTO_STATE=PROTO_CRC_CHECK;//PROTO_ADDR_CHECK;
				RECIEVED=1;

			  	ES=0;
			  	REN=0;  //recieve disable -��������� ��������� � �����		
			}
		}										
	}
//----------------------------��������----------------------------------------------------------------
	if(TI)
	{
		TI=0;
		 
		if(transf_count<buf_len)
		{
			if(transf_count<3)//�������� ���������
			{
				SBUF=TransferBuf[transf_count];			
				transf_count++;
			}
			else   //����...   ����������� 0 ����� 0xD7
			{
					if(CUT_OUT_NULL==0)
					{
						if(TransferBuf[transf_count]==(unsigned char)0xD7)//��������, ���  ,0xD7 ��� ������
						{			
							CUT_OUT_NULL=0x1;	
						}
						SBUF=TransferBuf[transf_count];			
						transf_count++;
					}
					else
					{
						SBUF=(unsigned char)0x0;
						CUT_OUT_NULL=0;		
					}	
			}	
		}
		else
		{
			transf_count=0;		//�������� �������
			//PROTO_STATE=PROTO_RESTART;
			PT_RESTART_OUT(pt_proto);
			CUT_OUT_NULL=0;
			DE_RE=0;//����� �� �����
			
		}					   
	}			
	EA=1;
	return;
}
 
//------------------------------------------------------------------------------
void Protocol_Init(void) //using 0
{
	TI=0;
	RI=0;
	//------------------------����� ������--------------------------------
	
//	CRC_ERR=0x0;	//������ crc
//	COMMAND_ERR=0x0;//���������������� �������
	
	TransferBuf=&RecieveBuf[0];	 //����� ������ =����� �������

//	PROTO_STATE=PROTO_RESTART;//������� ���������
	recieve_count=0x0;//������� ������ ������
	transf_count=0x0;//������� ������������ ������
	buf_len=0x0;//����� ������������� ������
	DE_RE=0;//����� �� �����
	CUT_OUT_NULL=0;
	PT_INIT(&pt_proto);
	return;
}
//-----------------------------------------------------------------------------
unsigned char Send_Info(void) //using 0    //������� ������ � ����������
{
	    unsigned char   idata i=0;
	   									
	   //��������� �����---
	   TransferBuf[0]=0x00;
	   TransferBuf[1]=0xD7;
	   TransferBuf[2]=0x29;
	   //------------------
	   TransferBuf[3]=ADRESS_DEV;  // ����� ����
	   TransferBuf[4]=GET_DEV_INFO_RESP;  // ��� ��������
	   TransferBuf[5]=25;//����� ���������� ����� �����

	   ((float*)(&TransferBuf[6]))[0]=skd.SKD_Set.SKD_Settings.calibr_high; //������� ������������� ��������
	   ((float*)(&TransferBuf[10]))[0]=skd.SKD_Set.SKD_Settings.calibr_low; //������ ������������� ��������
	   ((float*)(&TransferBuf[14]))[0]=skd.SKD_Set.SKD_Settings.diap_high; //������� �������
	   ((float*)(&TransferBuf[18]))[0]=skd.SKD_Set.SKD_Settings.diap_low; //������ �������

	   TransferBuf[22]= skd.SKD_Set.SKD_Settings.adc_diap;
	   ((unsigned int*)(&TransferBuf[23]))[0]=(skd.SKD_Set.SKD_Settings.indicate_time); //����� ���������, ��

	   for(i=0;i<5;i++)
	   {
	   		TransferBuf[25+i]=skd.SKD_Set.SKD_Settings.dev_id[i];	
	   }
	   

	   TransferBuf[30]=CRC_Check(&TransferBuf[1],29); // ������� ����������� �����
	   LED=0;
	return 31; 
}
//-----------------------------------------------------------------------------
unsigned char  Channel_Set_Parameters(void) //using 0 //���������� ��������� 
{
	    unsigned char   idata i=0;
	   									
	   (skd.SKD_Set.SKD_Settings.calibr_high)=((float*)(&RecieveBuf[6]))[0]; //������� ������������� ��������
	   (skd.SKD_Set.SKD_Settings.calibr_low)=((float*)(&RecieveBuf[10]))[0]; //������ ������������� ��������
	   (skd.SKD_Set.SKD_Settings.diap_high)=((float*)(&RecieveBuf[14]))[0]; //������� �������
	   (skd.SKD_Set.SKD_Settings.diap_low)=((float*)(&RecieveBuf[18]))[0]; //������ �������

	   skd.SKD_Set.SKD_Settings.adc_diap=RecieveBuf[22];
	   (skd.SKD_Set.SKD_Settings.indicate_time)=((unsigned int*)(&TransferBuf[23]))[0]; //����� ���������, ��

	   //setTimer(ADC_MEAN_TASK, 0, skd.SKD_Set.SKD_Settings.indicate_time>>3);

	   if(skd.SKD_Set.SKD_Settings.diap_high>=100.0 || skd.SKD_Set.SKD_Settings.diap_low<-100.0) //����������� ���������� �����
	   {
	   		LED_SetPoint(4);
	   }
	   else
	   {
	   		LED_SetPoint(3);
	   }

	   if(SHOW_VOLTAGE)//���� ���������� ���������� �� ����� �� ������� ����������
	   {
	   		LED_SetPoint(3);
	   }

	   for(i=0;i<5;i++)	  //�������� ID
	   {
	   		skd.SKD_Set.SKD_Settings.dev_id[i]=RecieveBuf[25+i];	
	   }
	   
	   Store_Channels_Data();//��������� ��������� � EEPROM
	   return Request_Error(FR_SUCCESFUL);
}
//-----------------------------------------------------------------------------
unsigned char Request_Error(unsigned char error_code) //using 0 //	��������� ������/�����;
{
	TransferBuf[0]=0x00;TransferBuf[1]=0xD7;TransferBuf[2]=0x29;
    TransferBuf[3]=ADRESS_DEV;  // ����� ����
    TransferBuf[7]=RecieveBuf[4]; // ��� ������� �������
    TransferBuf[4]=0xFF;  // ��� ��������

	TransferBuf[6]=0;//STATE_BYTE; // ���� ������� ����
    TransferBuf[8]=error_code;	 
    TransferBuf[5]=0x04;	  // ����� ������
    TransferBuf[9]=CRC_Check(TransferBuf,9);
	return 10;
}
//-----------------------------------------------------------------------------
void ProtoBufHandling(void) //using 0 //������� ��������� ��������� �������
{
  switch(RecieveBuf[4])
  {
//---------------------------------------
  	case GET_DEV_INFO_REQ:
	{
		buf_len=Send_Info();	
	}
	break;

	//-----------------------------------
	case CHANNEL_SET_PARAMETERS_REQ:
	{
			
		buf_len=Channel_Set_Parameters();

	}
	break;
	//-----------------------------------
    default:
	{
//       COMMAND_ERR=0x1;//�������������� �������
	   buf_len=Request_Error(FR_COMMAND_NOT_EXIST);
//	   PROTO_STATE=PROTO_ERR_HANDLING;//�� ���������� ������
    }								   
  }

  return;
}
//-----------------------------------------------------------------------------------
PT_THREAD(ProtoProcess(struct pt *pt))
 {
 unsigned char i=0;
 unsigned char  CRC=0x0;
  PT_BEGIN(pt);

  while(1) 
  {
  //----------restart------------
		recieve_count=0x0;//??
		REN=1;//recieve enqble
		DE_RE=0;//����� �� �����
		ES=1;
  //-----------------------------
	   PT_WAIT_UNTIL(pt,RECIEVED); //���� ������� �� �����
	   RECIEVED=0;
			
	   CRC=RecieveBuf[recieve_count-1];
				
		if(CRC_Check(&RecieveBuf,(recieve_count-CRC_LEN))!=CRC)
		{
			PT_RESTART(pt);//���� CRC �� �������-������������ ��������	 
		}
		PT_YIELD(pt);//����� ������ ��������� �����
  //-----------------------------
  		ProtoBufHandling();//��������� ��������� ���������	
		if(buf_len==0)//���� � ������ �����
		{
			PT_RESTART(pt);//������������ ��������	
		}
		else
		{
			DE_RE=1; //����������� RS485 � ����� ��������
							
			REN=0;	//������ ������-������ ��������
			transf_count=0;
			CUT_OUT_NULL=0;
			SBUF=TransferBuf[transf_count];//�������� �����, ��������� �������� �������
			transf_count++;//�������������� ������� ����������
			
			PT_RESTART(pt);//������������ ��������	
			
			ES=1; //������� ���������� ����		
		}

  //-----------------------------
  }

 PT_END(pt);
}
//-----------------------------------------------------------------------------------
//void ProtoProcess(void) //������� ������� 
//{
//	switch(PROTO_STATE)
//	{
////------------------------------------------
//		case(PROTO_RESTART):
//		{
//			recieve_count=0x0;//??
//		//	transf_count=0x0;
//
//
//			PROTO_STATE=PROTO_WAIT;//��������� � ��������� �������� 
//
//			REN=1;//recieve enqble
//			DE_RE=0;//����� �� �����
//			ES=1;	//���������� uart	
//		}
//		break;
////------------------------------------------
//		case(PROTO_WAIT)://�������� ������
//		{		
//			//�����
//			_nop_();
//		}
//		break;
////------------------------------------------
//
////------------------------------------------
//		case(PROTO_CRC_CHECK):// �������� CRC
//		{		 	
//			  
//					
//			CRC=RecieveBuf[recieve_count-1];
//					
//			if(CRC_Check(&RecieveBuf,(recieve_count-CRC_LEN))==CRC)
//			{
//				PROTO_STATE=PROTO_BUF_HANDLING;	 
//			}
//			else
//			{		
////				CRC_ERR=0x1;//������������ CRC
//	   			PROTO_STATE=PROTO_ERR_HANDLING;//�� ���������� ������								
//			} 	
//		}
//		break;
////------------------------------------------
//		case(PROTO_BUF_HANDLING):// ��������� ��������� ������
//		{			
//				ProtoBufHandling();//��������� ��������� ���������	
//				if(buf_len)//���� � ������ ��� �� ����
//					PROTO_STATE=PROTO_BUF_TRANSFER;//����� ��������� �������� ���������	
//				else
//					PROTO_STATE=PROTO_RESTART; //����� ������� ���������		
//		}
//		break;
////------------------------------------------
//		case(PROTO_BUF_TRANSFER):// �������� ������
//		{
//				DE_RE=1; //����������� RS485 � ����� ��������
//								
//				REN=0;	//������ ������-������ ��������
//				transf_count=0;
//				CUT_OUT_NULL=0;
//				SBUF=TransferBuf[transf_count];//�������� �����, ��������� �������� �������
//				transf_count++;//�������������� ������� ����������
//				PROTO_STATE=PROTO_WAIT;//��������� ������ � ������� � ����
//				
//				ES=1; //������� ���������� ����	
//		}
//		break;
////------------------------------------------
//		case(PROTO_ERR_HANDLING):// ��������� ������ // ����� �� �������������
//		{
//				//---��������� ������----
////				if(CRC_ERR)
//				{
//					//�������� ��������� ������
//
////					CRC_ERR=0x0;	//������ lrc
//				}
//
////				if(COMMAND_ERR)
//				{
//
////					COMMAND_ERR=0x0;//���������������� �������	
//				}
//				//-----------------------
//				
//				
//				PROTO_STATE=PROTO_RESTART;		//����� ������� � ���� ����� �������			
//		}
//		break;
////------------------------------------------
//		default:
//		{
//			PROTO_STATE==PROTO_RESTART;
//		}
//		break;
//	}
//
//	return;
//}
//-----------------------CRC------------------------------------------------------------
static unsigned char  CRC_Check( unsigned char xdata *Spool_pr,unsigned char Count_pr )  //���������
{
    unsigned char   idata CRC = 0;
	 	unsigned char xdata *Spool; 
	 	unsigned char idata Count ;
	
	Spool=Spool_pr;
	Count=Count_pr;

  		while(Count!=0x0)
        {
	        CRC = CRC ^ (*Spool++);//
	        // ����������� ����� ������
	        CRC = ((CRC & 0x01) ? (unsigned char)0x80: (unsigned char)0x00) | (unsigned char)(CRC >> 1);
	        // �������� ����� � 2 �� 5, ���� ��� 7 ����� 1
	        if (CRC & (unsigned char)0x80) CRC = CRC ^ (unsigned char)0x3C;
			Count--;
        }
    return CRC;
}

//-----------------------------------------------------------------------------------------------

