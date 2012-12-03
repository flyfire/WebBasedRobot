/****************************************************************
 * file:	main.c
 * date:	2012-08-11
 * author:      Xiaosong Chen
 * description: Main file of the project
 ***************************************************************/
#include "baselib.h"

#include "delay.h"
#include "nrf24l01.h"
#include "led.h"
#include "motor.h"
#include "keys.h"
#include "ISR.h"

#include "key_led_test.h"
#include "wireless_comm_test.h"
#include "walk_test.h"
#include "motor_encoder_test.h"
/*********************************************************************************************************
  ȫ�ֱ�������
*********************************************************************************************************/

extern unsigned long  LeftMotorStep ;
extern unsigned long  RightMotorStep ; 
extern L01Buf L01RcvBuf;

unsigned long distence = 0;
unsigned char TestNum=0;
unsigned char receive[32];

unsigned long banlance_l = 560;//��
unsigned long banlance_r = 500;//��

// ������������
unsigned char car_num = '1';
unsigned char info_cmd = '1';
unsigned char run_cmd = '2';
unsigned char forward_cmd = '3';
unsigned char backward_cmd = '4';
unsigned char turnleft_cmd = '5';
unsigned char turnright_cmd = '6';
unsigned char stop_cmd = '7';

void sysInit()
{
    //����ϵͳʱ��
     
    SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ); 
    IntMasterEnable();
}

unsigned char Scan(void)
{
  unsigned char rcvData[32];
  if(L01_Buf_Read(&L01RcvBuf,rcvData)==1)
  {
      int index;
      for(index=0; index < 32; index++)
      {
        receive[index] = rcvData[index];
      }
      return 1;
  }
  return 0;
}


void delay(unsigned long int d){        
  for(; d; --d);  
}

// �洢�����Ľṹ��
struct voltage{
  unsigned char a[2];
}x;

// ���ܣ� ������
struct voltage detectVoltage();

// ģ��ת����ʼ��
void ADCInit(void);

int main(void) 
{   
  // unsigned int i,j;///
   //unsigned char rcvData[32];
    sysInit();
    delay_ms(50);
    initLed();
    initMotor();
    initPWM();
    initKey();
    initSW();
   // initInfrared();
    ADCInit(); 
    initNrf24l01();
    initTimer();
    ledTurnon(LED0 | LED1 | LED2);
    TX_Mode(SSI1_BASE);
    RX_Mode(SSI0_BASE);

    while(1)
    {
      if(Scan()== 1)
      {
        if((car_num == receive[0]) || ('x' == receive[0]))
        {
          
          unsigned char response[32] = {0};
          int index;
          for(index=0; index < 32; index++)
            response[index] = '0';
          if('0' == receive[1]){
          if(info_cmd == receive[2])
          {
            /*
            response[0] = 'd';
            response[1] = car_num;
            response[2] = '0';
            response[3] = info_cmd;
            */
            // 32λchar�ַ�������ֵ��0λΪС�����
            response[0] = car_num;
            struct voltage y;
            y = detectVoltage();
            // 1, 2λΪ������ֵ
            response[1] = y.a[0];
            response[2] = y.a[1];
            
            // 3,7λΪ+��-����ʾ��ǰ�����(���ٶȣ����ٶ�)
            // 4,5,6λΪ�������ٶ�
            // 8,9,10λΪ�������ٶ�
            long leftMotor_L0;
            long leftMotor_L1;
            long rightMotor_L0;
            long rightMotor_L1;
            leftMotor_L0 = GPIOPinRead(GPIO_PORTG_BASE, MOTOR_L0); 
            leftMotor_L1 = GPIOPinRead(GPIO_PORTG_BASE, MOTOR_L1); 
            rightMotor_L0 = GPIOPinRead(GPIO_PORTD_BASE, MOTOR_L0); 
            rightMotor_L1 = GPIOPinRead(GPIO_PORTD_BASE, MOTOR_L1); 
            
            if (leftMotor_L0 !=0 && leftMotor_L1 == 0 && rightMotor_L0 != 0 && rightMotor_L1 == 0)//ǰ��
            {                                            
              response[3] = '+';//left motor speed
              response[4] = 48+banlance_l/100;//left motor speed
              response[5] = 48+(banlance_l%100)/10;//left motor speed
              response[6] = 48+banlance_l%10;//left motor speed                             
              response[7] = '+';//right motor speed
              response[8] = 48+banlance_r/100;//right motor speed
              response[9] = 48+(banlance_r%100)/10;//right motor speed
              response[10] = 48+banlance_r%10;//right motor speed	
            }
            else if (leftMotor_L0 ==0 && leftMotor_L1 != 0 && rightMotor_L0 == 0 && rightMotor_L1 != 0)//����
            {                                       
              response[3] = '-';//left motor speed
              response[4] = 48+banlance_l/100;//left motor speed
              response[5] = 48+(banlance_l%100)/10;//left motor speed
              response[6] = 48+banlance_l%10;//left motor speed                             
              response[7] = '-';//right motor speed
              response[8] = 48+banlance_r/100;//right motor speed
              response[9] = 48+(banlance_r%100)/10;//right motor speed
              response[10] = 48+banlance_r%10;//right motor speed	
            }	
            else if (leftMotor_L0 !=0 && leftMotor_L1 != 0 && rightMotor_L0 != 0 && rightMotor_L1 != 0)//ֹͣ
            {                                       
              response[3] = '+';//left motor speed
              response[4] = '0';//left motor speed
              response[5] = '0';//left motor speed
              response[6] = '0';//left motor speed                             
              response[7] = '+';//right motor speed
              response[8] = '0';//right motor speed
              response[9] = '0';//right motor speed
              response[10] = '0';//right motor speed	
            }
            NRF24L01_TxPacket(SSI1_BASE,response);
            delay_ms(8);
          }
          else if(run_cmd == receive[2])
          {
            forward();
          }
          else if(forward_cmd == receive[2])
          {
            forward();
          }
          else if(backward_cmd == receive[2])
          {
            backward();
          }
          else if(turnleft_cmd == receive[2])
          {
            turnLeft();
            delay(150000);
            forward();
          }
          else if(turnright_cmd == receive[2])
          {
            turnRight();
            delay(150000);
            forward();
          
          }
          else if(stop_cmd == receive[2])
          {
            stop();
          }
          }
        }
      }
    }
}


struct voltage detectVoltage()
{
  unsigned long ulVoltage;
  
  ADCProcessorTrigger(ADC_BASE, 0);                                   /*  ����������һ��A/Dת��       *///ADת����ģ��ת��
  //void ADCProcessorTrigger(unsigned long ulBase, unsigned long ulSequenceNum) ����һ�β������еĴ���������
  //ulBase��ADCģ��Ļ�ַ��ulSequenceNum�ǲ������еı��
  while (!ADCIntStatus(ADC_BASE, 0, false));                          /*  �ȴ�ת������                */
  //ע���while
  //unsigned long ADCIntStatus(unsigned long ulBase, unsigned long ulSequenceNum, tBoolean bMasked) ��ȡ��ǰ���ж�״̬
  //ulBase��ADCģ��Ļ�ַ��ulSequenceNum�ǲ������еı�ţ�bMasked����Ҫԭʼ���ж�״̬��False����Ҫ���ε��ж�״̬��True
  ADCIntClear(ADC_BASE, 0);                                           /*  ����жϱ�׼λ              */
  ADCSequenceDataGet(ADC_BASE, 0, &ulVoltage);                        /*  ��ȡת�����                *///����������Ϊ���ݴ�ŵĵ�ַ
  
  ulVoltage = ulVoltage * 3000 / 1023;                                /*  ����ʵ�ʼ�⵽�ĵ�ѹֵ(mV)  *///����һЩ��ѹ���㷨ʲô�ġ�������������о�
  ulVoltage = ulVoltage * 3 + 350;                                    /*  �����ص�ѹֵ(mV)          */
  
  x.a[0] = '0'+(ulVoltage % 10000) / 1000;
  x.a[1] = '0'+(ulVoltage % 1000 ) / 100;
  return x;    
}

void ADCInit (void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);                          /*  ʹ��ADCģ��                 *///ʹ��һ������
  SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS);                         /*  125KSps������               *///����ADC�������ʣ�KspsΪ�������ʵĵ�λΪÿ�����ǧ��
  
  ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);        /*  ����0Ϊ���������������ȼ�Ϊ0*///���ò������еĴ���Դ�����ȼ�
  ADCSequenceStepConfigure(ADC_BASE, 0, 0,
                           ADC_CTL_CH0 |//����ͨ�� 
                             ADC_CTL_IE  | //����һ���ж�
                               ADC_CTL_END);   //���е�ĩβ                           /*  ���ò������з������Ĳ���    */
  
  ADCHardwareOversampleConfigure(ADC_BASE, 16);                       /*  ����ADC����ƽ�����ƼĴ���   */
  //adc.h���У���pdf����û�У��ж�Ӧ��Software��
  ADCSequenceEnable(ADC_BASE, 0);                                     /*  ʹ�ܲ�������0               *///ʹ��һ����������
}

