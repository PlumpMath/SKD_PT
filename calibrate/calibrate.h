#ifndef CALIBRATE_H
#define CALIBRATE_H
#include "eeprom/eeprom.h"
#include "adc.h"
#include "skd.h"
//������� ���������� �� ���� ������
//------------------------------------------------------------
//static void SetFirstPoint(long ADC_Code,float val);	 //��������� ������ ����� -������ ��������� = 0
//static void SetSecondPoint(long ADC_Code,float val); //��������� ������ ����� -����	  ��������� = 1

//float GetFirstDensityPoint(void);
//float GetSecondDensityPoint(void);

void Calibrate(unsigned char point); //����� ������� ����������
//void RestoreCalibrate(void);		 //�������������� ����� ���������� �� EEPROM
float GetCalibrateVal(long ADC_Code);   //�������������� �������� ��� � �������� ��������� � ��\�^3

//float GetCompensedDensity(float pressure,float temp);   //�������������� �������� ��� � �������� ��������� � ��\�^3

//------------------------------------------------------------
#endif