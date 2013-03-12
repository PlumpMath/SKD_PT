#include "adc.h"
//-------------------------------------------
void ADC_Initialize() //using 0
{	
	ADCMODE&=0x0;
	ADC0CON1&=0x0;	
    ADC0CON2&=0x0;
	//ADCMODE=0xB;
//	ADCMODE=0x3; //chop enable

//	ADCMODE|=0x8;//CHOP DISABLE

    ADC0CON1|=BUF_BYPASS;
	ADC0CON1|=ADC_BIPOLAR;
	ADC0CON1|=RN_2560;


//	ADC_Restore_Settings();
	ADC_Set_Mode(ADC_CONTIN_CONV);

	ADC_Out_Freq(80);

	ADC0CON2=SENSOR_CHANNEL|(ADC0CON2&0xF8); //���������� �� ����� �������
	ADC0CON1=(ADC0CON1&0xF8)|(skd.SKD_Set.SKD_Settings.adc_diap & 0x7);//��������������� �������� ������ �������	

//	ADC0CON2&=0xF0;
//	ADC0CON2|=EXT_REF;//������� ������� �����
	EADC = 1;

	ADCMODE |= 0x20; //0010 0000 //ENABLE

	return;
}
//-------------------------------------------
void ADC_ISR(void) interrupt 6 //using 1
{
	if(skd.adc_sensor_queue_counter<SENSOR_QUEUE_LEN)
	{
		skd.ADC_SENSOR_UN[skd.adc_sensor_queue_counter].ADC_CHAR[0]=0x0;//������� ���������	�������
		skd.ADC_SENSOR_UN[skd.adc_sensor_queue_counter].ADC_CHAR[1]=ADC0H;
		skd.ADC_SENSOR_UN[skd.adc_sensor_queue_counter].ADC_CHAR[2]=ADC0M;
		//skd.ADC_SENSOR_UN[skd.adc_sensor_queue_counter].ADC_CHAR[3]=ADC0L;

		skd.adc_sensor_queue_counter++;

		if(skd.adc_sensor_queue_counter==SENSOR_QUEUE_LEN)
		{
			 ADC0CON1=(ADC0CON1&0xF8)|BRIGHTNESS_RANGE;//��������������� �������� ������ �������
			 ADC0CON2=BRIGHTNESS_CHANNEL|(ADC0CON2&0xF8); //���������� �� ����� �������		 
		}
	}
	else
	{
		skd.brightness=ADC0H;

		skd.adc_sensor_queue_counter=0x0;	//������� �������

		ADC0CON1=(ADC0CON1&0xF8)|(/*skd.SKD_Set.SKD_Settings.adc_diap*/LINE_SENSOR_RANGE & 0x7);//��������������� �������� ������ �������
		ADC0CON2=SENSOR_CHANNEL|(ADC0CON2&0xF8); //���������� �� ����� �������		
	}

	RDY0=0;

	return;
}
//--------------------------------------------------
void ADC_Set_Mode(unsigned char mode)	//using 0
{
	ADCMODE&=~0x7;
	ADCMODE|=mode;
	return;
}
//--------------------------------------------------
void ADC_Out_Freq(unsigned int freq)  // using 0
{
	SF=(unsigned char)(32768/3/8/freq);		  //������� ������ sinc3 ������� (���������)
	return;
}
 //------------------------------------------------------------------------------------------------------------------
 long Meaning( long mas[],unsigned char size) //using 2//������� ����������� �� ������������ ������� ����������
 {
 	 unsigned char xdata i=0;
	 long xdata result=0;
	 
	//------------------------------------------	
		for(i=0;i<size;i++)
		{
			result+=mas[i];
		}							
		result=(long)(result/(size));		
	return result;	   
 }
 //-----------------------------------------------------------------------------------------------------------------
 void Meaning_Process(void)//����������� �������, ���������� ������� �������� ��������
 {
 	//�����������
	skd.line_sensor=Meaning(&skd.ADC_SENSOR_UN,SENSOR_QUEUE_LEN);//������� ����������� �������� �� �������
 }
 //----------------------------------------------------------------------------------------------
//  #pragma OT(0,Speed) 
float GetVoltage(void)
{
	float ux=0;

	ux = (float)skd.line_sensor;
	ux = (float)(ux / (float)0x800000); // 2^(24-1)
	ux = ux - 1;
	ux = ux * (0.01*(1<<(skd.SKD_Set.SKD_Settings.adc_diap &0x7)))/*1.28*/; // 1.25*1.024 
	
	return ux;	
}
//-----------------------------------------------------------------------------------------------