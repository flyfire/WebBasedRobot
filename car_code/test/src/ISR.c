#include "ISR.h"

#include "baselib.h"

#include "delay.h"
#include "nrf24l01.h"
#include "led.h"
#include "motor.h"
#include "keys.h"

unsigned long DelayTime;
unsigned char Key1=0;
unsigned char Key0=0;

extern unsigned char TestNum;
extern unsigned long  LeftMotorStep;
extern unsigned long  RightMotorStep; 


//general function
void initTimer()
{
      //ʹ�ܶ�ʱ��0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 
      //���ö�ʱ��0Ϊ32λ���ڶ�ʱ��
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
	//���ö�ʱ��0��װ��ֵ
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/1000);  
	//���ö�ʱ���ж����ȼ�Ϊ1
	IntPrioritySet(INT_TIMER0A, 1<<5);
	//���ö�ʱ���ж�Ϊ��ʱ�ж�
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  
	//ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER0A);     
	TimerEnable(TIMER0_BASE, TIMER_A); 
    /*    
        //ʹ�ܶ�ʱ��1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); 
        //���ö�ʱ��1Ϊ32λ���ڶ�ʱ��
	TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);
	//���ö�ʱ��1��װ��ֵ
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()>>8);  
	//���ö�ʱ���ж����ȼ�Ϊ1
	IntPrioritySet(INT_TIMER1A, 1<<5);
	//���ö�ʱ���ж�Ϊ��ʱ�ж�
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);  
	//ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER1A);     
	TimerEnable(TIMER1_BASE, TIMER_A); */
}

/*RF receiving(nrf24l01.c) and key&led test(key_led_test.c)*/
void GPIO_Port_A_ISR(void)
{
  unsigned char sta;
  long intstatus = GPIOPinIntStatus(GPIO_PORTA_BASE, 0);
  if(intstatus & IRQ1)
  {
    sta=NRF24L01_Read_Reg(SSI0_BASE,READ_REG|STATUS);
    if(sta & RX_OK)
    {
       NRF24L01_RxPacket(SSI0_BASE);
    }
    NRF24L01_Write_Reg(SSI0_BASE,WRITE_REG|STATUS,intstatus);
  }
  GPIOPinIntClear(GPIO_PORTA_BASE,IRQ1);
}
/*RF receiving(nrf24l01.c) and key&led test(key_led_test.c)*/
void GPIO_Port_B_ISR(void)
{
  static unsigned char ledState=0;
  unsigned char sta;
  long intstatus = GPIOPinIntStatus(GPIO_PORTB_BASE, 0);
  if(intstatus & IRQ2)
  {
    sta=NRF24L01_Read_Reg(SSI1_BASE,READ_REG|STATUS);
    if(sta & RX_OK)
    {
       NRF24L01_RxPacket(SSI1_BASE);
    }
    NRF24L01_Write_Reg(SSI1_BASE,WRITE_REG|STATUS,intstatus);
  }
  else if(intstatus & KEY0)
  {
    Key0=1;
    if(TestNum==1)
    {
      if(ledState/3)//b0xx
      {
        switch(ledState%3)
        {
        case 0:
          ledTurnoff(LED0);
          break;
        case 1:
          ledTurnoff(LED1);
          break;
        case 2:
          ledTurnoff(LED2);
          break;
        }
        ledState-=3;
      }
      else        //b1xx
      {
        switch(ledState%3)
        {
        case 0:
          ledTurnon(LED0);
          break;
        case 1:
          ledTurnon(LED1);
          break;
        case 2:
          ledTurnon(LED2);
          break;
        default:
          break;
        }
        ledState+=3;
      }
    }
  }
  else if(intstatus & KEY1)
  {
    Key1=1;
    if(TestNum==1)
    {
      ledState=(ledState+1)%3;
    }
  }
  GPIOPinIntClear(GPIO_PORTB_BASE,KEY0 | KEY1 | IRQ2);
}


/*charging*/
void GPIO_Port_H_ISR(void)//PH2�ǳ�翪��
{
    return;
}

/*Delay counting(general)*/
//��ʱ��0���ڵ�������
void Timer0A_ISR(void)
{
    
    //����ж�Դ
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        DelayTime++;
	//����ת�ٴ�������,������������ת�ٲ���0�������̼���
        //displayLongData(banlance1);
   /*   
	if(LeftMotorStep < RightMotorStep-3)
	{
	  banlance1 = banlance1+delta;
          banlance2 = banlance2-delta;
	  PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, banlance1);//��
	  PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, banlance2);//��
          LeftMotorStep = 0;
          RightMotorStep = 0;
	}
	//����ת��С������,������������ת�ٲ���0�������̼���
	else if(LeftMotorStep > RightMotorStep+3)
	{
	  banlance1 = banlance1-delta;
	  banlance2 = banlance2+delta;
	  PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, banlance2);//Right
	  PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, banlance1);//Left
          LeftMotorStep = 0;
          RightMotorStep = 0;
	}
	//����ת�ٵ�������,��0�������̼���
	else
	{
          LeftMotorStep = 0;
          RightMotorStep = 0;
        }
	//ʹ�ܶ�ʱ��0�ж�
	TimerEnable(TIMER0_BASE, TIMER_A);
    */    
}


/*colliding switch*/
void GPIO_Port_C_ISR(void)
{
  //GPIOPinIntClear(GPIO_PORTC_BASE,KEY0 | KEY1);
 /* ledTurnover(LED1);
  backward();
  delay_ms(500);//����һ�ξ���
  turn();
  delay_ms(300);//ת��һ��ʱ�䣬��ʱ��
  forward();*/
  GPIOPinIntClear(GPIO_PORTC_BASE,KEY0 | KEY1);
}

/*infrared detection*/
void Timer1A_ISR(void)
{  
 // int i;
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  //TimerEnable(TIMER1_BASE, TIMER_A);
  //return;
/*
  for(i=0;i<3;i++){
    distence = getDistence(i);
    if(distence > 70){
       // ledTurnover(LED2);
        backward();
        delay_ms(500);//����һ�ξ���
        turn();
        delay_ms(300);//ת��һ��ʱ�䣬˳ʱ��
        forward();
        break;
    }
  }
  */
  //ʹ�ܶ�ʱ��0�ж�
 // TimerEnable(TIMER1_BASE, TIMER_A);
}

/*motor encoder*/

//��������
void GPIO_Port_D_ISR(void)
{
  GPIOPinIntClear(GPIO_PORTD_BASE,CODE_R);
  RightMotorStep++;//�����������̼���
}

//��������
void GPIO_Port_G_ISR(void)
{
  GPIOPinIntClear(GPIO_PORTG_BASE,CODE_L);
  LeftMotorStep++;
}