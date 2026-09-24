/*
 * USART_int.h
 *
 *  Created on: Aug 31, 2026
 *      Author: HP
 */

#ifndef MCAL_USART_USART_INT_H_
#define MCAL_USART_USART_INT_H_

void MUSART_vInit(void);

void MUSART_vSendData(u8 A_u8Data);
void MUSART_vSendString(char* A_u8ptrStr);
void MUSART_vSendDataAsynch(u8 A_u8Data);
void MUSART_vSendStringAsynch(char* A_u8ptrStr);


u8  MUSART_u8ReceiveData(void);
u8  MUSART_u8ReceiveSynch(u8* A_pu8Byte);

// INTERRUPTS
void MUSART_vEnable_TX_Interrupt(void);
void MUSART_vDisable_TX_Interrupt(void);
void MUSART_vEnable_TC_Interrupt(void);
void MUSART_vDisable_TC_Interrupt(void);
void MUSART_vEnable_RX_Interrupt(void);
void MUSART_vDisable_RX_Interrupt(void);


u8*  MUSART_u8ptrReceiveString(void);   // Check '\r' | '\n'
void MUSART_vUSART_CallBack(u8 A_u8USARTNo , void(*Fptr)(void));
u8   MUSART_u8Retreive_USART1_DataRegister(void);
void MUSART_vWrite_USART1_DataRegister(u8 A_u8Data);

#endif /* MCAL_USART_USART_INT_H_ */
