#include "delay.h"
/*ϵͳʱ��Ƶ��8M
-----------------------------------------------------------------------*/

void delay_1ms(void)                 //1ms????
{
  unsigned int i; 
  for(i=0;i<2000;i++)//forÿ��ѭ������ָ�
  {}
}
  
void delay_ms(unsigned int n)       //N ms????
{
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
}