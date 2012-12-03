#include "hw_types.h"
#include "timer.h"
#include "infraredkey.h"
#include "nrf24l01.h"
//�������
/*

unsigned int infData=0xffffffff;
volatile unsigned int currentTime=0;
unsigned char currentBit=0xff;
unsigned int data[321];
/////////////////////
void infraredkeyInit()
{
    
  //ʹ�ܶ�ʱ��0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 
      //���ö�ʱ��0Ϊ32λ���ڶ�ʱ��
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
	//���ö�ʱ��0��װ��ֵ
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/100000); //0.01ms 
	//���ö�ʱ���ж����ȼ�Ϊ1
	IntPrioritySet(INT_TIMER0A, 1<<5);
	//���ö�ʱ���ж�Ϊ��ʱ�ж�
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  
	//ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER0A);     
	TimerEnable(TIMER0_BASE, TIMER_A);
  
  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        
    //��������ж����ã���������Ч
    //�������Ϊ��4.5+0.6����λ��+��8λ������0.6+0.6Ϊ0,1.6+0.6Ϊ1���ȸߺ�ͣ�+��������+8λ������+�����뷴��    
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN); 
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    GPIOPinIntEnable(GPIO_PORTA_BASE, GPIO_PIN_0);
    IntEnable(INT_GPIOA);
    
    
    
}
void Timer0A_ISR(void)
{
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  currentTime++;
}

unsigned char getInfKey()
{
  if(currentBit!=32)
  {
    return 0xff;
  }
  currentBit=0xff;
  return (unsigned char)(infData>>16);
}

void GPIO_Port_A_ISR(void)
{
  static unsigned int k=0;
  static unsigned int lastTime=0;
  unsigned int timeGap=currentTime-lastTime;
  lastTime=currentTime;
  data[k]=timeGap;
  k++;
  if(k>=320)
  {
    k=320;
  }
 
  if(timeGap>450 && timeGap<550)
  {
    currentBit=0;
  }
  else if(currentBit!=0xff && timeGap>90 && timeGap<130)//112(0)
  {
    infData =infData& (~(0x00000001<<currentBit));
    currentBit++;
  }
  else if(currentBit!=0xff && timeGap>200 && timeGap<250)//225
  {
    infData =infData | (0x00000001<<currentBit);
    currentBit++;
  }
  else
  {
    currentBit=0xff;
  }
  if(currentBit==32)
  {
    //���������
  }
  GPIOPinIntClear(GPIO_PORTA_BASE,GPIO_PIN_0);
}

*/