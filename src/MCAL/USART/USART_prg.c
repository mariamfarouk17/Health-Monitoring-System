/*
 * USART_prg.c
 *
 *  Created on: Aug 31, 2026
 *      Author: HP
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "USART_int.h"
#include "USART_prv.h"

volatile u8 G_Data;
volatile char* G_u8ptrString = NULL;
volatile u8 G_u8Index = 0;
volatile u8 G_u8StringBusy = 0;

volatile u8 G_u8Buffer[100];


void MUSART_vInit(void)
{
	// OVERSAMPLE BY 16
	CLR_BIT(USART1->USARTx_CR1 , 15);  // OVER8 BIT

	// DATA LENGTH 8
	CLR_BIT(USART1->USARTx_CR1 , 12);  // M BIT

	// NO PARITY
	CLR_BIT(USART1->USARTx_CR1 , 10);  // PCE BIT

	// BAUD RATE 9600
	// APB2 = 16 MHz
	USART1->USARTx_BRR = ((104U << 4) | 3U);   // 104 >> Clk = 16MHz

	// 1 STOP BIT
	CLR_BIT(USART1->USARTx_CR2 , 12);  // STOP BIT1
	CLR_BIT(USART1->USARTx_CR2 , 13);  // STOP BIT2

	// ENABLE USART
	SET_BIT(USART1->USARTx_CR1 , 13);  // UE BIT

	// ENABLE TRANSMITTER
	SET_BIT(USART1->USARTx_CR1 , 3);   // TE BIT

	// ENABLE RECEIVER
	SET_BIT(USART1->USARTx_CR1 , 2);   // RE BIT
}

void MUSART_vSendData(u8 A_u8Data)
{
	// CHECK (TXE FLAG != 1)
	while(!GET_BIT(USART1->USARTx_SR , 7));    // TXE BIT

	USART1->USARTx_DR = A_u8Data;

	// WAIT WHILE TC = 0 (TRANSMISSION NOT COMPLETE)
	while(!GET_BIT(USART1->USARTx_SR , 6));    // TC BIT

	// TC = 1
	// CLEAR TC FOR NEXT TRANSMISSION
	CLR_BIT(USART1->USARTx_SR , 6);
}

void MUSART_vSendString(char* A_u8ptrStr)
{
    while (*A_u8ptrStr != '\0')
    {
        MUSART_vSendData(*A_u8ptrStr);

        A_u8ptrStr++;
    }
}

void MUSART_vSendDataAsynch(u8 A_u8Data)
{
	G_Data = A_u8Data;
	MUSART_vEnable_TX_Interrupt();

}

void MUSART_vSendStringAsynch(char* A_u8ptrStr)
{
//    if ((A_u8ptrStr != NULL) && (G_u8StringBusy == 0))
//    {
//        G_u8ptrString = A_u8ptrStr;
//        G_u8Index = 0;
//        G_u8StringBusy = 1;
//
//        MUSART_vEnable_TX_Interrupt();
//    }

	G_u8ptrString = A_u8ptrStr;
	G_u8Index = 0;
	//G_u8StringBusy = 1;

	MUSART_vEnable_TX_Interrupt();
}


u8 MUSART_u8ReceiveData(void)
{
	// WAIT WHILE RXNE = 0 (NO DATA RECEIVED)
	while(!GET_BIT(USART1->USARTx_SR , 5));    // RXNE BIT

	// RXNE = 1 >> DATA RECEIVED
	// RETURN DATA
	return USART1->USARTx_DR;
}

u8  MUSART_u8ReceiveSynch(u8* A_pu8Byte)
{
	u8 L_u8status = 1 ;
	if( GET_BIT(USART1->USARTx_SR, 5) == 1 )
	{
		*A_pu8Byte = USART1->USARTx_DR ;
	}
	else
	{
		L_u8status = 0;
	}

	return L_u8status ;
}

void MUSART_vEnable_TX_Interrupt(void)
{
	SET_BIT(USART1->USARTx_CR1 , 7);
}

void MUSART_vDisable_TX_Interrupt(void)
{
	CLR_BIT(USART1->USARTx_CR1 , 7);
}
void MUSART_vEnable_TC_Interrupt(void)
{
	SET_BIT(USART1->USARTx_CR1 , 6);
}

void MUSART_vDisable_TC_Interrupt(void)
{
	CLR_BIT(USART1->USARTx_CR1 , 6);
}

void MUSART_vEnable_RX_Interrupt(void)
{
	SET_BIT(USART1->USARTx_CR1 , 5);
}

void MUSART_vDisable_RX_Interrupt(void)
{
	CLR_BIT(USART1->USARTx_CR1 , 5);
}



u8*  MUSART_u8ptrReceiveString(void)   // Check '\r' | '\n'
{
    u8 L_u8Index = 0;
    u8 L_u8Data;

    while (L_u8Index < 99)
    {
        L_u8Data = MUSART_u8ReceiveData();

        if ((L_u8Data == '\n') || (L_u8Data == '\r'))
        {
            break;
        }

        G_u8Buffer[L_u8Index] = L_u8Data;
        L_u8Index++;
    }

    G_u8Buffer[L_u8Index] = '\0';

    return G_u8Buffer;
}


void USART1_IRQHandler(void)
{
    /* ================= TXE Interrupt ================= */
    if ((GET_BIT(USART1->USARTx_SR, 7) == 1) &&
        (GET_BIT(USART1->USARTx_CR1, 7) == 1))
    {
    	if(G_u8ptrString[G_u8Index] != '\0')
        {
    		USART1->USARTx_DR = G_u8ptrString[G_u8Index];
    		G_u8Index ++ ;
        }
    	else
    	{
    		MUSART_vDisable_TX_Interrupt();
    		MUSART_vEnable_TC_Interrupt();
    	}
    }

    /* ================= TC Interrupt ================= */

    if ((GET_BIT(USART1->USARTx_SR, 6) == 1) &&
        (GET_BIT(USART1->USARTx_CR1, 6) == 1))
    {
        /* Clear TC */
        CLR_BIT(USART1->USARTx_SR, 6);

        /* Disable TC interrupt */
        MUSART_vDisable_TC_Interrupt();

        //G_u8StringBusy = 0;

    }
}

