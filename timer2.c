#include "timer2.h"
//-------------------------------------------------------
void Timer2_Initialize(void) //using 0// ������������� ������� 2-�������� ������� ��� �����������
{
    ET2 = 1;
   	TL2 = 0x0;
	TH2 = 0x0;
	RCAP2L = 0x0;
    RCAP2H = 0x0;
//	TR2=1;
	return;
}
//-------------------------------------------------------