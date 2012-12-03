#ifndef NRF24L01_H
#define NRF24L01_H

#include "hw_memmap.h"
#include "hw_types.h"
#include "interrupt.h"   
#include "hw_ints.h" 
#include "debug.h"
#include "delay.h"
#include "gpio.h" 
#include "ssi.h"


#define CE1 GPIO_PIN_5  //PORT C
#define CSN1 GPIO_PIN_4  //PORT C
#define IRQ1 GPIO_PIN_6  //PORT A

#define CE2 GPIO_PIN_3  //PORT B
#define CSN2 GPIO_PIN_1  //PORT E
#define IRQ2 GPIO_PIN_2 //PORT B

#define Set_nRF1_CE    GPIOPinWrite(GPIO_PORTC_BASE, CE1, 0xff)
#define Clr_nRF1_CE    GPIOPinWrite(GPIO_PORTC_BASE, CE1, 0x00)
#define Set_nRF1_CSN   GPIOPinWrite(GPIO_PORTC_BASE, CSN1, 0xff)
#define Clr_nRF1_CSN   GPIOPinWrite(GPIO_PORTC_BASE, CSN1, 0x00)

#define Set_nRF2_CE    GPIOPinWrite(GPIO_PORTB_BASE, CE2, 0xff)
#define Clr_nRF2_CE    GPIOPinWrite(GPIO_PORTB_BASE, CE2, 0x00)
#define Set_nRF2_CSN   GPIOPinWrite(GPIO_PORTE_BASE, CSN2, 0xff)
#define Clr_nRF2_CSN   GPIOPinWrite(GPIO_PORTE_BASE, CSN2, 0x00)


//***************************************************//
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address//״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                                    //bit5:���ݷ�������ж�;bit6:���������ж�;

#define MAX_TX  	0x10        //�ﵽ����ʹ����ж�
#define TX_OK   	0x20        //TX��������ж�
#define RX_OK   	0x40       //���յ������ж�

#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define MAX_RT  	0x10  // Max #of TX retrans interrupt
#define TX_DS   	0x20  // TX data sent interrupt
#define RX_DR   	0x40  // RX data received

#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command



//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    3   //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    3   //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  //20�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  //20�ֽڵ��û����ݿ��
#define POWER 0

#define   L01_BUF_SIZE 64
#define   PACKAGE_SIZE 32
typedef struct  //���ͺͽ��ջ��������壨���Σ�
{
	unsigned char   p_u8Data[L01_BUF_SIZE][PACKAGE_SIZE];
        unsigned char   c_u8Num;
	unsigned char   i_u8SavePtr;
	unsigned char   i_u8GetPtr;
}L01Buf;

void initNrf24l01();
unsigned char NRF24L01_Write_Reg(unsigned long ulBase, unsigned char reg, unsigned char value);
unsigned char NRF24L01_Read_Reg(unsigned long ulBase, unsigned char reg);
unsigned char NRF24L01_Read_Buf(unsigned long ulBase, unsigned char reg,unsigned char *pBuf,unsigned char len);
unsigned char NRF24L01_Write_Buf(unsigned long ulBase, unsigned char reg, unsigned char *pBuf, unsigned char len);
unsigned char NRF24L01_TxPacket(unsigned long ulBase, unsigned char *txbuf);
unsigned char NRF24L01_RxPacket(unsigned long ulBase);
void RX_Mode(unsigned long ulBase);
void TX_Mode(unsigned long ulBase);
unsigned char L01_Buf_Read( L01Buf *buf, unsigned char *pValue );
unsigned char L01_Buf_Write( L01Buf *buf, unsigned char *pValue );

#endif