#include "baselib.h"
#include "keys.h"

//����������not��ײ����
void initKey()
{
    //��ʼ������˿�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        
    //�����ж����ã��½��ش���    
    GPIODirModeSet(GPIO_PORTB_BASE, KEY0 | KEY1, GPIO_DIR_MODE_IN); 
    GPIOPadConfigSet(GPIO_PORTB_BASE, KEY0 | KEY1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTB_BASE, KEY0 | KEY1, GPIO_FALLING_EDGE);
    GPIOPinIntEnable(GPIO_PORTB_BASE, KEY0 | KEY1);
    IntEnable(INT_GPIOB);
}

//��ײ����
void initSW()
{
    //��ʼ������˿�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
        
    //�����ж����ã��½��ش���
    GPIODirModeSet(GPIO_PORTC_BASE, SW1 | SW2, GPIO_DIR_MODE_IN); 
    GPIOPadConfigSet(GPIO_PORTC_BASE, SW1 | SW2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTC_BASE, SW1 | SW2, GPIO_FALLING_EDGE);
    GPIOPinIntEnable(GPIO_PORTC_BASE, SW1 | SW2);
    IntEnable(INT_GPIOC);
}


void initInfrared()
{
    //���ú��ⷢ��GPIO���ƹܽ�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIODirModeSet(GPIO_PORTA_BASE, LED_FRONT_RIGHT, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTA_BASE, LED_FRONT_RIGHT, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIODirModeSet(GPIO_PORTG_BASE, LED_FRONT_LEFT | LED_FRONT_MIDDLE | LED_BACK_LEFT | LED_BACK_MIDDLE, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTG_BASE, LED_FRONT_LEFT | LED_FRONT_MIDDLE | LED_BACK_LEFT | LED_BACK_MIDDLE, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIODirModeSet(GPIO_PORTF_BASE, LED_BACK_RIGHT, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTF_BASE, LED_BACK_RIGHT, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    //ADC����
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);                
    SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS);
    //SysCtlPeripheralEnable(SYSCTL_ADCSPEED_1MSPS); 
    ADCHardwareOversampleConfigure(ADC_BASE, 0);
    ADCSoftwareOversampleConfigure(ADC_BASE,0, 8);
}

unsigned long getDistence(int num)
{
    unsigned long ulVoltage1;
    unsigned long ulVoltage2;
    
    ulVoltage1=0;
    ulVoltage2=0;
    
    //ADCSoftwareOversampleConfigure(ADC_BASE,3, 8);
    ADCSequenceDisable(ADC_BASE, 0);
    ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    
    switch (num) {
    case 0://ǰ��
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_FRONT_LEFT  |  ADC_CTL_IE   |  ADC_CTL_END);
        break;
    case 1://ǰ��
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_FRONT_MIDDLE  |  ADC_CTL_IE   |  ADC_CTL_END);
        break;
    case 2://ǰ��
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_FRONT_RIGHT  |  ADC_CTL_IE   |  ADC_CTL_END);
        break;
    case 3://����
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_BACK_RIGHT  |  ADC_CTL_IE   |  ADC_CTL_END);
        break;
    case 4://����
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_BACK_MIDDLE  |  ADC_CTL_IE   |  ADC_CTL_END);
        break;
    case 5://����
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_BACK_LEFT  |  ADC_CTL_IE   |  ADC_CTL_END);
        break;
    default:
        break;
    }
    //ADCSequenceStepConfigure(ADC_BASE, 0, 0, num  |  ADC_CTL_IE   |  ADC_CTL_END);
    //ADCSequenceStepConfigure(ADC_BASE, 3, 0, num  |  ADC_CTL_END);
    ADCSequenceEnable(ADC_BASE, 0);
    
    ADCProcessorTrigger(ADC_BASE, 0);                                   
    while (!ADCIntStatus(ADC_BASE, 0, false));                          
    ADCIntClear(ADC_BASE, 0);                                           
    ADCSequenceDataGet(ADC_BASE, 0, &ulVoltage1);
    //ulVoltage1 = (ulVoltage1 & 0x3ff);
    
    //return ulVoltage1;

    switch (num) {
    case 0://ǰ��
        GPIOPinWrite(GPIO_PORTG_BASE, LED_FRONT_LEFT, 0xff);
        break;
    case 1://ǰ��
        GPIOPinWrite(GPIO_PORTG_BASE, LED_FRONT_MIDDLE, 0xff);
        break;
    case 2://ǰ��
        GPIOPinWrite(GPIO_PORTA_BASE, LED_FRONT_RIGHT, 0xff);
        break;
    case 3://����
        GPIOPinWrite(GPIO_PORTF_BASE, LED_BACK_RIGHT, 0xff);
        break;
    case 4://����
        GPIOPinWrite(GPIO_PORTG_BASE, LED_BACK_MIDDLE, 0xff);
        break;
    case 5://����
        GPIOPinWrite(GPIO_PORTG_BASE, LED_BACK_LEFT, 0xff);
        break;
    default:
        break;
    }
    
    delay_ms(50);
    ADCProcessorTrigger(ADC_BASE, 0);                                   
    while (!ADCIntStatus(ADC_BASE, 0, false));                          
    ADCIntClear(ADC_BASE, 0);                                           
    ADCSequenceDataGet(ADC_BASE, 0, &ulVoltage2);
    
    //ulVoltage2 = (ulVoltage2 & 0x3ff);
    switch (num) {
    case 0:
        GPIOPinWrite(GPIO_PORTG_BASE, LED_FRONT_LEFT, 0x00);
        break;
    case 1:
        GPIOPinWrite(GPIO_PORTG_BASE, LED_FRONT_MIDDLE, 0x00);
        break;
    case 2:
        GPIOPinWrite(GPIO_PORTA_BASE, LED_FRONT_RIGHT, 0x00);
        break;
    case 3:
        GPIOPinWrite(GPIO_PORTF_BASE, LED_BACK_RIGHT, 0x00);
        break;
    case 4:
        GPIOPinWrite(GPIO_PORTG_BASE, LED_BACK_MIDDLE, 0x00);
        break;
    case 5:
        GPIOPinWrite(GPIO_PORTG_BASE, LED_BACK_LEFT, 0x00);
        break;
    default:
        break;
    }
    if (ulVoltage1 >= ulVoltage2) ulVoltage1 -= ulVoltage2;
    else ulVoltage1 = 0;
    return ulVoltage1;
}