#include "nrf24l01.h"

#define NUM_SSI_DATA 3

unsigned char P0_TX_ADDRESS[TX_ADR_WIDTH]={0xe7,0xe7,0xe7}; //���͵�ַ
unsigned char P0_RX_ADDRESS[RX_ADR_WIDTH]={0xe7,0xe7,0xe7}; //���յ�ַ

unsigned long ulDataTx[NUM_SSI_DATA];
unsigned long ulDataRx[NUM_SSI_DATA];
unsigned long ulindex =0;

unsigned char L01IRQSign=0;
L01Buf L01RcvBuf;

void initSSI0()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  
  GPIOPinConfigure(GPIO_PA2_SSI0CLK);//��ò������һ��С���ϵĳ���
  GPIOPinConfigure(GPIO_PA4_SSI0RX);
  GPIOPinConfigure(GPIO_PA5_SSI0TX);
  
  GPIOPinTypeSSI(GPIO_PORTA_BASE ,GPIO_PIN_2);
  GPIOPinTypeSSI(GPIO_PORTA_BASE ,GPIO_PIN_4);
  GPIOPinTypeSSI(GPIO_PORTA_BASE ,GPIO_PIN_5); 
  
  long rate;
  rate =  SysCtlClockGet();
  
  SSIConfigSetExpClk(SSI0_BASE, rate, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, rate>>1 , 8);
  
  SSIEnable(SSI0_BASE);

    //
    // Read any residual data from the SSI port.  This makes sure the receive
    // FIFOs are empty, so we don't read any unwanted junk.  This is done here
    // because the SPI SSI mode is full-duplex, which allows you to send and
    // receive at the same time.  The SSIDataGetNonBlocking function returns
    // "true" when data was returned, and "false" when no data was returned.
    // The "non-blocking" function checks if there is any data in the receive
    // FIFO and does not "hang" if there isn't.
    //t
    while(SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx[0]))
    {
    }
}

void initSSI1()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  
  GPIOPinConfigure(GPIO_PE0_SSI1CLK);
  //GPIOPinConfigure(GPIO_PE1_SSI1FSS);
  GPIOPinConfigure(GPIO_PE2_SSI1RX);
  GPIOPinConfigure(GPIO_PE3_SSI1TX);
  
  GPIOPinTypeSSI(GPIO_PORTE_BASE ,GPIO_PIN_0);
  //GPIOPinTypeSSI(GPIO_PORTE_BASE ,GPIO_PIN_1);
  GPIOPinTypeSSI(GPIO_PORTE_BASE ,GPIO_PIN_2);
  GPIOPinTypeSSI(GPIO_PORTE_BASE ,GPIO_PIN_3); 
  
  long rate;
  rate =  SysCtlClockGet();
  
  SSIConfigSetExpClk(SSI1_BASE, rate, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, rate>>1 , 8);
  
  SSIEnable(SSI1_BASE);
  
  while(SSIDataGetNonBlocking(SSI1_BASE, &ulDataRx[0]))
    {
    }
}

void initControlPins0()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA | SYSCTL_PERIPH_GPIOC);
  
  GPIODirModeSet(GPIO_PORTC_BASE,  CE1 | CSN1, GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTC_BASE, CE1 | CSN1, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
  
  GPIODirModeSet(GPIO_PORTA_BASE, IRQ1, GPIO_DIR_MODE_IN); 
  GPIOPadConfigSet(GPIO_PORTA_BASE, IRQ1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOIntTypeSet(GPIO_PORTA_BASE, IRQ1, GPIO_FALLING_EDGE);
  GPIOPinIntEnable(GPIO_PORTA_BASE, IRQ1);
   GPIOPinIntClear(GPIO_PORTA_BASE, IRQ1);
  IntEnable(INT_GPIOA);
}

void initControlPins1()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB | SYSCTL_PERIPH_GPIOE);
  
  GPIODirModeSet(GPIO_PORTB_BASE,  CE2, GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTB_BASE, CE2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
  
  GPIODirModeSet(GPIO_PORTE_BASE,  CSN2, GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTE_BASE, CSN2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
  
  GPIODirModeSet(GPIO_PORTB_BASE, IRQ2, GPIO_DIR_MODE_IN); 
  GPIOPadConfigSet(GPIO_PORTB_BASE, IRQ2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOIntTypeSet(GPIO_PORTB_BASE, IRQ2, GPIO_FALLING_EDGE);
  GPIOPinIntEnable(GPIO_PORTB_BASE, IRQ2);
  GPIOPinIntClear(GPIO_PORTB_BASE, IRQ2);
  IntEnable(INT_GPIOB);
}

/*==================================================
#define nRF1_CEH    GPIOPinWrite(GPIO_PORTC_BASE, CE1, 0xff)
#define nRF1_CEL    GPIOPinWrite(GPIO_PORTC_BASE, CE1, 0x00)
#define nRF1_CSNH   GPIOPinWrite(GPIO_PORTC_BASE, CSN1, 0xff)
#define nRF1_CSNL   GPIOPinWrite(GPIO_PORTC_BASE, CSN1, 0x00)
====================================================*/
void initNrf24l01()
{  
  L01IRQSign=0;
  L01RcvBuf.c_u8Num=0;
  L01RcvBuf.i_u8SavePtr=L01RcvBuf.i_u8GetPtr=0;
  
  initSSI0();
  initControlPins0();
  
  initSSI1();
  initControlPins1();
}

unsigned char SSI_RW_Byte(unsigned long ulBase, unsigned char byte)
{
  unsigned char ucVal; 
  unsigned long ulVal; 
  
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  SSIDataPut(ulBase, byte);
  while(SSIBusy(ulBase));
  SSIDataGet(ulBase, &ulVal);
  ucVal = (unsigned char)ulVal;
  return ucVal;
}

//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
unsigned char NRF24L01_Write_Reg(unsigned long ulBase, unsigned char reg, unsigned char value)
{
  unsigned char status;
  
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)             //ʹ��SPI����
    Clr_nRF1_CSN;
  else
    Clr_nRF2_CSN;
  
  status =SSI_RW_Byte(ulBase,reg);   //���ͼĴ����� 
  SSI_RW_Byte(ulBase, value);         //д��Ĵ�����ֵ
  
  if(ulBase == SSI0_BASE)             //��ֹSPI����
    Set_nRF1_CSN;
  else
    Set_nRF2_CSN;
  
  return(status);       	      //����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
unsigned char NRF24L01_Read_Reg(unsigned long ulBase, unsigned char reg)
{
  unsigned char reg_val;	
  
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)             //ʹ��SPI����
    Clr_nRF1_CSN;
  else
    Clr_nRF2_CSN;	
  
  SSI_RW_Byte(ulBase,reg);                   //���ͼĴ�����
  reg_val=SSI_RW_Byte(ulBase,0XFF);          //��ȡ�Ĵ�������

  if(ulBase == SSI0_BASE)             //��ֹSPI����
    Set_nRF1_CSN;
  else
    Set_nRF2_CSN;
  
  return(reg_val);                    //����״ֵ̬
}


//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ�����ַ
//*pBuf:����������ָ��
//len:�������ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
unsigned char NRF24L01_Read_Buf(unsigned long ulBase, unsigned char reg,unsigned char *pBuf,unsigned char len)
{
  unsigned char status,u8_ctr;
  
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)                    //ʹ��SPI����
    Clr_nRF1_CSN;
  else
    Clr_nRF2_CSN;
  
  status=SSI_RW_Byte(ulBase,reg);            //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
  pBuf[u8_ctr]=SSI_RW_Byte(ulBase,0XFF);     //��������
  
  if(ulBase == SSI0_BASE)             //��ֹSPI����
    Set_nRF1_CSN;
  else
    Set_nRF2_CSN;
  
  return status;                             //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
unsigned char NRF24L01_Write_Buf(unsigned long ulBase, unsigned char reg, unsigned char *pBuf, unsigned char len)
{
  unsigned char status,u8_ctr;
  
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)                    //ʹ��SPI����
    Clr_nRF1_CSN;
  else
    Clr_nRF2_CSN;
  
  status = SSI_RW_Byte(ulBase,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
    SSI_RW_Byte(ulBase,*pBuf++); //д������
  
  if(ulBase == SSI0_BASE)             //��ֹSPI����
    Set_nRF1_CSN;
  else
    Set_nRF2_CSN;
  
  return status;          //����д����״ֵ̬
}


/************************************************
 * Description: Tx a packet with retreat on
 *
 * Arguments:
 * 	ulBase -- indicate which nRF24L01 is used.
 *      txbuf -- pointer of data that need to be transmited
 * Return:
 * 	TRUE -- If phy in idle state
 * 	FALSE -- If tx in progress
 *
 * Date: 2010-05-21
 ***********************************************/
unsigned char TxPacket_retreat(unsigned long ulBase, unsigned char *txbuf)
{
  unsigned char sta;
    
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)                    //ʹ��SPI����
    Clr_nRF1_CE;
  else
    Clr_nRF2_CE;
  
  NRF24L01_Write_Buf(ulBase,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
  	
  while(!NRF24L01_Read_Reg(ulBase,READ_REG|CD));
  
  
  
  if(ulBase == SSI0_BASE)                    //��������
    Set_nRF1_CE;
  else
    Set_nRF2_CE;
  
  //while(NRF24L01_IRQ!=0);//�ȴ��������
  if(ulBase == SSI0_BASE)
    while((GPIOPinRead(GPIO_PORTA_BASE,IRQ1) & IRQ1) !=0);//�ȴ��������
  else
    while((GPIOPinRead(GPIO_PORTB_BASE,IRQ2) & IRQ1) !=0);
  
  
  sta=NRF24L01_Read_Reg(ulBase,READ_REG|STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
  
  NRF24L01_Write_Reg(ulBase,WRITE_REG|STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
  
  if(sta&MAX_TX)//�ﵽ����ط�����
  {
    NRF24L01_Write_Reg(ulBase,FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
    return MAX_TX; 
  }
  if(sta&TX_OK)//�������
  {
  return TX_OK;
  }
  return 0xff;//����ԭ����ʧ��

}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
unsigned char NRF24L01_TxPacket(unsigned long ulBase, unsigned char *txbuf)
{
  unsigned char sta;
  
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)                    //ʹ��SPI����
    Clr_nRF1_CE;
  else
    Clr_nRF2_CE;
  
  NRF24L01_Write_Buf(ulBase,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
  
  if(ulBase == SSI0_BASE)                    //��������
    Set_nRF1_CE;
  else
    Set_nRF2_CE;
  
  //while(NRF24L01_IRQ!=0);//�ȴ��������
  if(ulBase == SSI0_BASE)
    while((GPIOPinRead(GPIO_PORTA_BASE,IRQ1) & IRQ1) !=0);//�ȴ��������
  else
    while((GPIOPinRead(GPIO_PORTB_BASE,IRQ2) & IRQ2) !=0);
  
  
  sta=NRF24L01_Read_Reg(ulBase,READ_REG|STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
  NRF24L01_Write_Reg(ulBase,WRITE_REG|STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
  
 /* if(sta&MAX_TX)//�ﵽ����ط�����
  {
    NRF24L01_Write_Reg(ulBase,FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
    return MAX_TX; 
  }
  if(sta&TX_OK)//�������
  {
    return TX_OK;
  }*/
  return 0xff;//����ԭ����ʧ��
}

//����NRF24L01����һ������
//rxbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
unsigned char NRF24L01_RxPacket(unsigned long ulBase)
{
  unsigned char sta;		    							   
  unsigned char rxbuf[32];
  sta=NRF24L01_Read_Reg(ulBase,READ_REG | STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
  NRF24L01_Write_Reg(ulBase, WRITE_REG | STATUS, sta); //���TX_DS��MAX_RT�жϱ�־
  if(sta&RX_OK)//���յ�����
  {
    NRF24L01_Read_Buf(ulBase,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
    NRF24L01_Write_Reg(ulBase,FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
    L01_Buf_Write( &L01RcvBuf, rxbuf );
    return 0; 
  }	   
  return 1;//û�յ��κ�����

}	
					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(unsigned long ulBase)
{
//	Clr_NRF24L01_CE;
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)                    //ʹ��SPI����
    Clr_nRF1_CE;
  else
    Clr_nRF2_CE;
  NRF24L01_Write_Reg(ulBase,WRITE_REG|SETUP_AW, RX_ADR_WIDTH-2);     
  NRF24L01_Write_Buf(ulBase,WRITE_REG|RX_ADDR_P0,(unsigned char*)P0_RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
   
  NRF24L01_Write_Reg(ulBase,WRITE_REG|EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  NRF24L01_Write_Reg(ulBase,WRITE_REG|RF_CH,POWER);	     //����RFͨ��Ƶ��2.4GHZ		  
  NRF24L01_Write_Reg(ulBase,WRITE_REG|RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��,32�ֽ� 	    
  NRF24L01_Write_Reg(ulBase,WRITE_REG|RF_SETUP,0x0f);//����RX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(ulBase,WRITE_REG|CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  
  NRF24L01_Write_Reg(ulBase,WRITE_REG|EN_AA,0x00);    //��ֹͨ��0���Զ�Ӧ��  
  NRF24L01_Write_Reg(ulBase,WRITE_REG|SETUP_RETR,0x00);    //�Զ��ط�����Ϊ0  
  
  if(ulBase == SSI0_BASE)                    //CEΪ��,�������ģʽ
    Set_nRF1_CE;
  else
    Set_nRF2_CE;
}
							 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR(�������Ŵ�����)
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(unsigned long ulBase)
{														 
  ASSERT((ulBase == SSI0_BASE) || (ulBase == SSI1_BASE));
  
  if(ulBase == SSI0_BASE)                    //ʹ��SPI����
    Clr_nRF1_CE;
  else
    Clr_nRF2_CE;
  NRF24L01_Write_Reg(ulBase,WRITE_REG|SETUP_AW, RX_ADR_WIDTH-2); 
  NRF24L01_Write_Buf(ulBase,WRITE_REG|TX_ADDR,(unsigned char*)P0_TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 	  
  
  NRF24L01_Write_Reg(ulBase,WRITE_REG|RF_CH,POWER);       //����RFͨ��Ϊ40
  NRF24L01_Write_Reg(ulBase,WRITE_REG|RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(ulBase,WRITE_REG|CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
//	Set_NRF24L01_CE;//CEΪ��,10us����������
  
  NRF24L01_Write_Reg(ulBase,WRITE_REG|EN_AA,0x00);     //��ֹͨ��0���Զ�Ӧ��      
  NRF24L01_Write_Reg(ulBase,WRITE_REG|SETUP_RETR,0x00);//�����Զ��ط��ر�
  if(ulBase == SSI0_BASE)                    //CEΪ��,�������ģʽ
    Set_nRF1_CE;
  else
    Set_nRF2_CE;
}

/******************************************************************************
*				bool	L01_Buf_Read( L01Buf buf, u8 *value )
*
* Function Name:  L01_Buf_Read
*
* Description  :  ����������ڶ���L01�����������Ķ�����
*
* Input        :  L01Buf buf,��ȡֵ L01RcvBuf��L01SendBuf     
*
* Output       :  value, ��ȡ��32λ����
*
* Return       :  ���Ƿ���Ч
*******************************************************************************/
unsigned char L01_Buf_Read( L01Buf *buf, unsigned char *pValue )
{
	int i;
	if(buf->c_u8Num==0)
	{
		return 0;
	}
	
	for(i=0;i<PACKAGE_SIZE;i++)
	{
		*(pValue+i)=buf->p_u8Data[buf->i_u8GetPtr][i];
	}
	buf->i_u8GetPtr = (buf->i_u8GetPtr+1)&(L01_BUF_SIZE-1);	
	buf->c_u8Num--;
	return 1;

}

/******************************************************************************
*				bool L01_Buf_Write( L01Buf buf, u8 *pValue )
*
* Function Name:  USART_Buf_Write
*
* Description  :  ����������ڶ���������������д����
*
* Input        :  L01Buf buf,��ȡֵ L01RcvBuf��L01SendBuf     
*
* Output       :  value, д���32λ����
*
* Return       :  д�Ƿ���Ч
*******************************************************************************/
unsigned char L01_Buf_Write( L01Buf *buf, unsigned char *pValue )
{
	int i;
	if(buf->c_u8Num==L01_BUF_SIZE)
	{
		return 0;
	}
	
	for(i=0;i<PACKAGE_SIZE;i++)
	{
		buf->p_u8Data[buf->i_u8SavePtr][i] = *(pValue+i);
	}	
	buf->i_u8SavePtr = (buf->i_u8SavePtr+1)&(L01_BUF_SIZE-1); 
	buf->c_u8Num++;
	return 1;
}

/*
//�ɴ���
void GPIO_Port_B_ISR(void)
{
  unsigned char sta;
  unsigned char recvbuf[32];
 // unsigned long a=0;
  long intstatus = GPIOPinIntStatus(GPIO_PORTB_BASE, 0);
  
  if(intstatus & IRQ2){
      sta=NRF24L01_Read_Reg(SSI1_BASE,READ_REG|STATUS);
      if(sta & RX_OK){
        if(NRF24L01_RxPacket(SSI1_BASE,recvbuf) == 0){
          if(recvbuf[0]==0xff){
            //ledTurnover(LED1);
            //trx_width-=4;
            RX_Mode(SSI1_BASE);            
          }else if(recvbuf[0]==0xf8){
            sendback=1;
          }else if(recvbuf[0]==0x00){
         //   ledTurnover(LED2);
            rightrate[recvbuf[0]]++;
          }
        }
      }
      NRF24L01_Write_Reg(SSI1_BASE,WRITE_REG|STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
  }
  
  GPIOPinIntClear(GPIO_PORTB_BASE,SW1 | SW2 | IRQ2);
}
*/