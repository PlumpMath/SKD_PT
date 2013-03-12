#ifndef CHANNELS_H
#define CHANNELS_H
#include "eeprom/eeprom.h"
#include "preferences.h"
//---------------------------------------
#define CHANNEL_NUMBER	12//���������� �������
//---------------------------------------
struct Channel
{
	unsigned char number ;	  	// ����� ������ 
	
	union 
	{
		struct
		{
			unsigned char type;		  // ��� ������
			unsigned char modific;	  // ����������� ������
			unsigned char state_byte_1;	// ���� ��������� ������
			unsigned char state_byte_2; 
		}set;
	
		unsigned long serialize;//��������������� ����� ���������
	} settings;
	
	unsigned long channel_data;		  // �������� ������
	unsigned long channel_data_calibrate;//�������� ������ �������������

	union
	{
		struct
		{
			unsigned char calibrate;//���� ��������������� ������
		/*	unsigned long first_point_x;  //����� ���������� ������
			unsigned long second_point_x;
			unsigned long first_point_y;
			unsigned long second_point_y;*/

			float K;//
			float C;//y=kx+c

			
		} cal;
		unsigned long serialize[3];//��������������� ����� ���������
	} calibrate;
	
} ;
//---------------------------------------
void ChannelsInit(void);//������������� ��������� �������
void Store_Channels_Data(void);//�������� ������ ������� � ����
void Restore_Channels_Data(void);//����������� ������ ������� �� ����

//---------------------------------------
#endif