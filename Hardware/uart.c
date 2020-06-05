//#include <stdio.h>
//#include <stdint.h>
#include "uart.h"	
//#include "stm32f1xx_hal_conf.h"
//#include "stm32f1xx_hal_uart.h"
//#include "stm32f103xb.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0       // for KEIL
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	  

uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

//UART_HandleTypeDef
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			// if(USART_RX_STA&0x4000)//接收到了0x0d
			// {
			// 	if(aRxBuffer[0]!=0x0a) USART_RX_STA=0;//接收错误,重新开始
			// 	else USART_RX_STA|=0x8000;	//接收完成了 
			// }
			// else //收到0x0A 或 0x0D, 都表示接收完成
			{	
                //USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
				if( (aRxBuffer[0]==0x0D) || (aRxBuffer[0]==0x0A)) USART_RX_STA|=0x8000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1)) USART_RX_STA=0; //接收数据错误,重新开始接收	  
				}
			}
		}
	}
}

#endif