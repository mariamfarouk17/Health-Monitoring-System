/*
 * USART_int.h
 */

#ifndef MCAL_USART_USART_INT_H_
#define MCAL_USART_USART_INT_H_

#include "../../LIB/STD_TYPES.h"

/* ==================== USART1 APIs ==================== */
void MUSART1_vInit(void);
void MUSART1_vSendData(u8 A_u8Data);
u8   MUSART1_u8ReceiveData(void);
u8   MUSART1_vReceive_synch(u8* A_pu8Byte);
void MUSART1_vSendString(char* A_u8ptrStr);
u8*  MUSART1_u8ptrReceiveString(void);
void MUSART1_vSendStringAsynch(u8* A_pu8String, u8 A_u8Length);

void MUSART1_vEnable_TX_Interrupt(void);
void MUSART1_vDisable_TX_Interrupt(void);
void MUSART1_vEnable_TC_Interrupt(void);
void MUSART1_vDisable_TC_Interrupt(void);
void MUSART1_vEnable_RX_Interrupt(void);
void MUSART1_vDisable_RX_Interrupt(void);

u8   MUSART1_u8Retreive_DataRegister(void);
void MUSART1_vWrite_DataRegister(u8 A_u8Data);

/* ==================== USART2 APIs ==================== */
void MUSART2_vInit(void);
void MUSART2_vSendData(u8 A_u8Data);
u8   MUSART2_u8ReceiveData(void);
u8   MUSART2_vReceive_synch(u8* A_pu8Byte);
void MUSART2_vSendString(char* A_u8ptrStr);
u8*  MUSART2_u8ptrReceiveString(void);
void MUSART2_vSendStringAsynch(u8* A_pu8String, u8 A_u8Length);

void MUSART2_vEnable_TX_Interrupt(void);
void MUSART2_vDisable_TX_Interrupt(void);
void MUSART2_vEnable_TC_Interrupt(void);
void MUSART2_vDisable_TC_Interrupt(void);
void MUSART2_vEnable_RX_Interrupt(void);
void MUSART2_vDisable_RX_Interrupt(void);

u8   MUSART2_u8Retreive_DataRegister(void);
void MUSART2_vWrite_DataRegister(u8 A_u8Data);

/* Callback Registration */
void MUSART_vUSARTCallBack(u8 A_u8USARTNo, void(*Fptr)(void));

#endif /* MCAL_USART_USART_INT_H_ */
