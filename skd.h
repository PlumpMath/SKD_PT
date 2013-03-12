#ifndef SKD_H
#define SKD_H

#include "eeprom/eeprom.h"
#include "preferences.h"

#define SENSOR_QUEUE_LEN 8

#define CALIBR_LOW   0.0
#define CALIBR_HIGH 35.0

#define DIAP_LOW   0.0
#define DIAP_HIGH 40.0

#define INDICATE_TIME	50

//----------------��������� �������� SKD
struct SKD
{
	union  
	{
		struct
		{

		   long adc_calibr_low;//������ �������� ��� ��� ����������
		   float calibr_low;//������ ������������� ��������
		   
		   long adc_calibr_high;//������� �������� ��� ��� ����������
		   float calibr_high;//������� ������������� ��������
		  
		   float diap_low;//������ ������� ���������
		   float diap_high;//������� ������� ���������
		   
		   unsigned char adc_diap;//�������� ��. ���������� ���
		   unsigned char dev_id[5];//id � ������� xx.yyy
		   unsigned int indicate_time;//����� ����� ���������, ms


		}SKD_Settings;
	
		unsigned long serialize[8];//��������������� ����� ���������
	} SKD_Set;

   // long brightness_channel;   //����� ��� �������

	union  //����� ���������� ���  ������
	{
				 long ADC_LONG	;
				 char ADC_CHAR[4];
	} ADC_SENSOR_UN[SENSOR_QUEUE_LEN];

	unsigned char adc_sensor_queue_counter;//������� ��������� ������ ������� ���


	long line_sensor;
	unsigned char brightness;
	

};

//--------------------------------------
void SKDInit(void);//������������� ��������� �������
void Store_Channels_Data(void);//�������� ������ ������� � ����
void Restore_Channels_Data(void);//����������� ������ ������� �� ����
//--------------------------------------

#endif