#include "keyboard.h"
#include "led_process.h"
#include "calibrate/calibrate.h"


sbit BUTTON1=P2^4;
sbit BUTTON2=P2^5;

 unsigned char  SHOW_VOLTAGE=0;

extern  struct SKD xdata skd ;

PT_THREAD(Keyboard_Process(struct pt *pt))//������� ��������� ����������
{
 PT_BEGIN(pt);
  while(1) 
  {
  //------------------������ ����� -----------------------------

		if(SHOW_VOLTAGE)	 //�� ���������
		{
			  PT_RESTART(pt);
		}
		
				
		PT_DELAY(pt,50);	//�������� ���������
	
		if(!BUTTON1)
		{
			PT_DELAY(pt,200);	//�������� ���������
			if(!BUTTON1)
			{
			  	Calibrate(1);
				PT_WAIT_WHILE(pt,BUTTON1);

			}			
		}

		if(!BUTTON2)
		{
			PT_DELAY(pt,200);	//�������� ���������
			if(!BUTTON2)
			{
			  	Calibrate(2);
				PT_WAIT_WHILE(pt,BUTTON2);
			}	
		}
		PT_RESTART(pt);		
  }

  PT_END(pt);
}
//-------------------------------------------------------
