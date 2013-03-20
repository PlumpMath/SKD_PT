#ifndef CALIBRATE_H
#define CALIBRATE_H
#include "eeprom/eeprom.h"
#include "adc.h"
#include "skd.h"
//������� ���������� �� ���� ������
//------------------------------------------------------------

void Calibrate(unsigned char point); //����� ������� ����������
float GetCalibrateVal(long ADC_Code);   //�������������� �������� ��� � �������� ��������� � ��\�^3
//------------------------------------------------------------
#endif