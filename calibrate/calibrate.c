#include "calibrate.h"
extern  struct SKD xdata skd ;
//------------------------------------------------------------
void Calibrate(unsigned char point) //����� ������� ����������
{
	switch(point)
	{
		case 1:
		{
			skd.SKD_Set.SKD_Settings.adc_calibr_low=skd.line_sensor;	
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.adc_calibr_low,1,SETTINGS_ADDR);
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.calibr_low,1,SETTINGS_ADDR+1);
		}
		break;

		case 2:
		{
			skd.SKD_Set.SKD_Settings.adc_calibr_high=skd.line_sensor;
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.adc_calibr_high,1,SETTINGS_ADDR+2);
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.calibr_high,1,SETTINGS_ADDR+3);
		}
		break;

		default:
		{
			return;
		}
	}
}
//------------------------------------------------------------
float GetCalibrateVal(long ADC_Code)   //�������������� �������� ��� � ������������� ��������	 ??? ��������� && debug
{
  	float   val=0;//���������

	val=(((float)((float)(ADC_Code-skd.SKD_Set.SKD_Settings.adc_calibr_low)/(float)(skd.SKD_Set.SKD_Settings.adc_calibr_high-skd.SKD_Set.SKD_Settings.adc_calibr_low)))*(float)(skd.SKD_Set.SKD_Settings.calibr_high-skd.SKD_Set.SKD_Settings.calibr_low))+skd.SKD_Set.SKD_Settings.calibr_low; //������� ���������� �� 2� ������
	return val;
}
//---------------------------------------------------------------
