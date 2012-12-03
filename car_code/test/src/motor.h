#ifndef MOTOR_H
#define MOTOR_H


//����ͷ�ļ�
#include "Pwm.h"


//�ܽ���������μ�ԭ��ͼ
#define PWM_L     GPIO_PIN_2//PG�˿�     ����PWM����
#define MOTOR_L0  GPIO_PIN_0//PG�˿�     ���ֿ�������1
#define MOTOR_L1  GPIO_PIN_1//PG�˿�     ���ֿ�������2
//#define SEN_L     ADC_CTL_CH6   //SEN_Lû�нӡ������ӵ���
#define CODE_L    GPIO_PIN_3//PG�˿�     ��������

#define PWM_R     GPIO_PIN_1//PD�˿�     ���ֿ���ʹ��
#define MOTOR_R0  GPIO_PIN_3//PD�˿�     ���ֿ�������1
#define MOTOR_R1  GPIO_PIN_2//PD�˿�     ���ֿ�������2
#define SEN_R     ADC_CTL_CH3
#define CODE_R    GPIO_PIN_0//PD�˿�     ��������


//GPIO��ʼ��
void initMotor();

//PWM��ʼ��
void initPWM();

void leftspeedSet(char direction,unsigned long lspeed);//-100~100
void rightspeedSet(char direction,unsigned long rspeed);

//ǰ��
void forward();

//����
void backward();
//ֹͣ
void stop();
//ת��,��ʱ��
void turn();
void stepAdjust();
#endif

