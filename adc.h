#ifndef ADC_H
#define ADC_H

#include <ADuC845.h>
#include "preferences.h"
#include "eeprom/eeprom.h"
#include "proto_uso/channels.h"
#include "skd.h"
#include <math.h>
//#include <stdio.h>

//-----------------------------------------------------------
#define ADC_CHANNELS_NUM	8//���������� ���������� ������� �����������
//#define ADC_MODE_ZEROCAL_INT 0x04 // 0000 0100
//#define ADC_MODE_FULLCAL_INT 0x05 // 0000 0101
//---------------------������� ���������---------------------
#define RN_20 	0x0//��������� +-20��
#define RN_40	0x1
#define RN_80	0x2
#define RN_160	0x3
#define RN_320	0x4
#define RN_640	0x5
#define RN_1280	0x6
#define RN_2560	0x7//2.56 V

//-----------------------������------------------------------
#define ADC_PWR_DWN			0x0 //power down
#define	ADC_IDLE_MD			0x1	//idle
#define	ADC_SINGLE_CONV		0x2	//single conversion
#define ADC_CONTIN_CONV		0x3	//continous conversion
#define ADC_INTERNAL_CAL	0x4//internal calibration
#define ADC_FULL_INT_CAL	0x5 //fullscale calibration
#define ADC_SYSTEM_CAL		0x6//internal system calibration
#define ADC_FULL_SYS_CAL	0x7 //fullscale system calibration
//-----------------------------�����-------------------------
#define AIN1_AINCOM 	0x1
#define AIN2_AINCOM		0x2
#define AIN3_AINCOM		0x3
#define AIN4_AINCOM		0x4
#define AIN5_AINCOM 	0x5
#define AIN6_AINCOM		0x6
#define AIN7_AINCOM		0x7
#define AIN8_AINCOM		0x8

#define AIN1_AIN2 		0xA
#define AIN3_AIN4		0xB
#define AIN5_AIN6		0xC
#define AIN7_AIN8		0xD
//-----------------------------------------------------------
#define ADC_BUF_SIZE 8//������� ���������� ���
//-----------------------------------------------------------
#define BUF_ENABLE 	0x0//�������� ������� ������	
#define	BUF_BYPASS	0x80 //��������� ������� ������(�� ��������� 0�0-��������)
//-----------------------------------------------------------
#define INTERN_REF	0x0 //���������� ����� ���	 1.25�
#define EXT_REF		0x40//������� ����� ��� 2.5 �
//-----------------------------------------------------------
#define ADC_UNIPOLAR	0x20//����������� �����
#define ADC_BIPOLAR		0x0//���������� �����
//-----------------------------------------------------------
#define SENSOR_CHANNEL		AIN7_AIN8//������ ��� ������� � �������...���������
#define BRIGHTNESS_CHANNEL	AIN5_AIN6
#define BRIGHTNESS_RANGE		RN_2560//����� -2.56V
#define LINE_SENSOR_RANGE	RN_2560//����� -2.56V
//-----------------------------------------------------------
//-----------------------------------------------------------
void ADC_Initialize();
void ADC_Run();
void ADC_Stop();
void ADC_ISR(void);
void ADC_Set_Range(unsigned char range);   //40mV-2.56V
void ADC_Set_Mode(unsigned char mode);	   //
void ADC_Set_Input(unsigned char input);   //diff out
void ADC_Out_Freq(unsigned int freq);	   //DECIMATION
void ADC_Calibrate(char mode);//���������� ���
//void ADC_Set_Settings(unsigned char settings[]);

float GetVoltage(void);

void ADC_Restore_Settings(void);//�������������� �������� ���

long Meaning(long mas[],unsigned char size);//������� ����������� �� ������������ ������� ����������

void Meaning_Process(void);//����������� �������, ���������� ������� �������� ��������
long GetSensorVal(void) ;//���������� �������� ��� �������
//unsigned char Out_Task(void);//����� ������
//----------------------------------------------------------------------------------
extern struct SKD xdata skd;  //��������� ���
//---------------------
#endif