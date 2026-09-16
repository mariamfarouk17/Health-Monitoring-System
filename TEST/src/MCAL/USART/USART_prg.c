/*
 * USART_prg.c
 *
 *  Created on: Sep 16, 2026
 *      Author: PC
 */

/* 1. Inclusion of Essential Libraries */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

/* 2. Inclusion of Driver Files */
#include "USART_int.h"
#include "USART_prv.h"

/* Global Array for Callback Functions (USART1, USART2, USART6) */
static void (*G_Fpt[3])(void) = {NULL, NULL, NULL};

/* Global Variables for USART1 Asynchronous Operations */
static u8* G_pu8TxBuffer1 = NULL;
static u8  G_u8TxLength1  = 0U;
static u8  G_u8TxIndex1   = 0U;
static u8  G_Buffer1[50];

/* Global Variables for USART2 Asynchronous Operations */
static u8* G_pu8TxBuffer2 = NULL;
static u8  G_u8TxLength2  = 0U;
static u8  G_u8TxIndex2   = 0U;
static u8  G_Buffer2[50];


/* =====================================================================
 *                          USART1 Implementation
 * ===================================================================== */

void MUSART1_vInit(void)
{
    /* Oversample by 16 */
    CLR_BIT(USART1->CR1, 15);

    /* Word length 8 bits */
    CLR_BIT(USART1->CR1, 12);

    /* No parity */
    CLR_BIT(USART1->CR1, 10);

    /* Baud Rate 9600 @ 16 MHz Clock -> BRR = (104 << 4) | 3 */
    USART1->BRR = (104U << 4) | 3U;

    /* 1 Stop bit */
    CLR_BIT(USART1->CR2, 12);
    CLR_BIT(USART1->CR2, 13);

    /* Enable Transmitter & Receiver */
    SET_BIT(USART1->CR1, 3);
    SET_BIT(USART1->CR1, 2);

    /* Enable USART1 */
    SET_BIT(USART1->CR1, 13);
}

void MUSART1_vSendData(u8 A_u8Data)
{
    while (GET_BIT(USART1->SR, 7) == 0);
    USART1->DR = A_u8Data;
    while (GET_BIT(USART1->SR, 6) == 0);
    CLR_BIT(USART1->SR, 6);
}

u8 MUSART1_u8ReceiveData(void)
{
    while (GET_BIT(USART1->SR, 5) == 0);
    return (u8)USART1->DR;
}

u8 MUSART1_vReceive_synch(u8* A_pu8Byte)
{
    u8 L_u8status = 1U;
    if (GET_BIT(USART1->SR, 5) == 1)
    {
        *A_pu8Byte = (u8)USART1->DR;
    }
    else
    {
        L_u8status = 0U;
    }
    return L_u8status;
}

void MUSART1_vSendString(char* A_u8ptrStr)
{
    u8 L_u8Index = 0U;
    if (A_u8ptrStr != NULL)
    {
        while (A_u8ptrStr[L_u8Index] != '\0')
        {
            MUSART1_vSendData((u8)A_u8ptrStr[L_u8Index]);
            L_u8Index++;
        }
    }
}

u8* MUSART1_u8ptrReceiveString(void)
{
    u8 L_u8index = 0U;
    u8 ch;

    while (L_u8index < 49U)
    {
        ch = MUSART1_u8ReceiveData();
        if (ch == '\r' || ch == '\n')
        {
            break;
        }
        G_Buffer1[L_u8index] = ch;
        L_u8index++;
    }
    G_Buffer1[L_u8index] = '\0';
    return G_Buffer1;
}

void MUSART1_vEnable_TX_Interrupt(void)  { SET_BIT(USART1->CR1, 7); }
void MUSART1_vDisable_TX_Interrupt(void) { CLR_BIT(USART1->CR1, 7); }
void MUSART1_vEnable_TC_Interrupt(void)  { SET_BIT(USART1->CR1, 6); }
void MUSART1_vDisable_TC_Interrupt(void) { CLR_BIT(USART1->CR1, 6); }
void MUSART1_vEnable_RX_Interrupt(void)  { SET_BIT(USART1->CR1, 5); }
void MUSART1_vDisable_RX_Interrupt(void) { CLR_BIT(USART1->CR1, 5); }

void MUSART1_vWrite_DataRegister(u8 A_u8Data)
{
    USART1->DR = A_u8Data;
}

u8 MUSART1_u8Retreive_DataRegister(void)
{
    return (u8)USART1->DR;
}

void MUSART1_vSendStringAsynch(u8* A_pu8String, u8 A_u8Length)
{
    if (A_pu8String != NULL)
    {
        G_pu8TxBuffer1 = A_pu8String;
        G_u8TxLength1  = A_u8Length;
        G_u8TxIndex1   = 0U;

        MUSART1_vEnable_TX_Interrupt();
    }
}


/* =====================================================================
 *                          USART2 Implementation
 * ===================================================================== */

void MUSART2_vInit(void)
{
    /* Oversample by 16 */
    CLR_BIT(USART2->CR1, 15);

    /* Word length 8 bits */
    CLR_BIT(USART2->CR1, 12);

    /* No parity */
    CLR_BIT(USART2->CR1, 10);

    /* Baud Rate 9600 @ 16 MHz Clock -> BRR = (104 << 4) | 3 */
    USART2->BRR = (104U << 4) | 3U;

    /* 1 Stop bit */
    CLR_BIT(USART2->CR2, 12);
    CLR_BIT(USART2->CR2, 13);

    /* Enable Transmitter & Receiver */
    SET_BIT(USART2->CR1, 3);
    SET_BIT(USART2->CR1, 2);

    /* Enable USART2 */
    SET_BIT(USART2->CR1, 13);
}

void MUSART2_vSendData(u8 A_u8Data)
{
    while (GET_BIT(USART2->SR, 7) == 0);
    USART2->DR = A_u8Data;
    while (GET_BIT(USART2->SR, 6) == 0);
    CLR_BIT(USART2->SR, 6);
}

u8 MUSART2_u8ReceiveData(void)
{
    while (GET_BIT(USART2->SR, 5) == 0);
    return (u8)USART2->DR;
}

u8 MUSART2_vReceive_synch(u8* A_pu8Byte)
{
    u8 L_u8status = 1U;
    if (GET_BIT(USART2->SR, 5) == 1)
    {
        *A_pu8Byte = (u8)USART2->DR;
    }
    else
    {
        L_u8status = 0U;
    }
    return L_u8status;
}

void MUSART2_vSendString(char* A_u8ptrStr)
{
    u8 L_u8Index = 0U;
    if (A_u8ptrStr != NULL)
    {
        while (A_u8ptrStr[L_u8Index] != '\0')
        {
            MUSART2_vSendData((u8)A_u8ptrStr[L_u8Index]);
            L_u8Index++;
        }
    }
}

u8* MUSART2_u8ptrReceiveString(void)
{
    u8 L_u8index = 0U;
    u8 ch;

    while (L_u8index < 49U)
    {
        ch = MUSART2_u8ReceiveData();
        if (ch == '\r' || ch == '\n')
        {
            break;
        }
        G_Buffer2[L_u8index] = ch;
        L_u8index++;
    }
    G_Buffer2[L_u8index] = '\0';
    return G_Buffer2;
}

void MUSART2_vEnable_TX_Interrupt(void)  { SET_BIT(USART2->CR1, 7); }
void MUSART2_vDisable_TX_Interrupt(void) { CLR_BIT(USART2->CR1, 7); }
void MUSART2_vEnable_TC_Interrupt(void)  { SET_BIT(USART2->CR1, 6); }
void MUSART2_vDisable_TC_Interrupt(void) { CLR_BIT(USART2->CR1, 6); }
void MUSART2_vEnable_RX_Interrupt(void)  { SET_BIT(USART2->CR1, 5); }
void MUSART2_vDisable_RX_Interrupt(void) { CLR_BIT(USART2->CR1, 5); }

void MUSART2_vWrite_DataRegister(u8 A_u8Data)
{
    USART2->DR = A_u8Data;
}

u8 MUSART2_u8Retreive_DataRegister(void)
{
    return (u8)USART2->DR;
}

void MUSART2_vSendStringAsynch(u8* A_pu8String, u8 A_u8Length)
{
    if (A_pu8String != NULL)
    {
        G_pu8TxBuffer2 = A_pu8String;
        G_u8TxLength2  = A_u8Length;
        G_u8TxIndex2   = 0U;

        MUSART2_vEnable_TX_Interrupt();
    }
}


/* =====================================================================
 *                          General APIs & ISRs
 * ===================================================================== */

void MUSART_vUSARTCallBack(u8 A_u8USARTNo, void(*Fptr)(void))
{
    u8 L_u8index = (A_u8USARTNo == 1U) ? 0U : (A_u8USARTNo == 2U) ? 1U : 2U;
    G_Fpt[L_u8index] = Fptr;
}

void USART1_IRQHandler(void)
{
    /* TXE Interrupt */
    if ((GET_BIT(USART1->SR, 7) == 1) && (GET_BIT(USART1->CR1, 7) == 1))
    {
        if (G_pu8TxBuffer1 != NULL && G_u8TxIndex1 < G_u8TxLength1)
        {
            USART1->DR = G_pu8TxBuffer1[G_u8TxIndex1];
            G_u8TxIndex1++;
        }
        else
        {
            MUSART1_vDisable_TX_Interrupt();
            MUSART1_vEnable_TC_Interrupt();
        }
    }

    /* TC Interrupt */
    if ((GET_BIT(USART1->SR, 6) == 1) && (GET_BIT(USART1->CR1, 6) == 1))
    {
        CLR_BIT(USART1->SR, 6);
        MUSART1_vDisable_TC_Interrupt();

        if (G_Fpt[0] != NULL)
        {
            G_Fpt[0]();
        }
    }

    /* RXNE Interrupt */
    if ((GET_BIT(USART1->SR, 5) == 1) && (GET_BIT(USART1->CR1, 5) == 1))
    {
        if (G_Fpt[0] != NULL)
        {
            G_Fpt[0]();
        }
    }
}

void USART2_IRQHandler(void)
{
    /* TXE Interrupt */
    if ((GET_BIT(USART2->SR, 7) == 1) && (GET_BIT(USART2->CR1, 7) == 1))
    {
        if (G_pu8TxBuffer2 != NULL && G_u8TxIndex2 < G_u8TxLength2)
        {
            USART2->DR = G_pu8TxBuffer2[G_u8TxIndex2];
            G_u8TxIndex2++;
        }
        else
        {
            MUSART2_vDisable_TX_Interrupt();
            MUSART2_vEnable_TC_Interrupt();
        }
    }

    /* TC Interrupt */
    if ((GET_BIT(USART2->SR, 6) == 1) && (GET_BIT(USART2->CR1, 6) == 1))
    {
        CLR_BIT(USART2->SR, 6);
        MUSART2_vDisable_TC_Interrupt();

        if (G_Fpt[1] != NULL)
        {
            G_Fpt[1]();
        }
    }

    /* RXNE Interrupt */
    if ((GET_BIT(USART2->SR, 5) == 1) && (GET_BIT(USART2->CR1, 5) == 1))
    {
        if (G_Fpt[1] != NULL)
        {
            G_Fpt[1]();
        }
    }
}


