/*
 * I2C_prg.c
 *
 *  Created on: Sep 7, 2026
 *      Author: HP
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../RCC/RCC_int.h"
#include "../GPIO/GPIO_int.h"

#include "I2C_int.h"
#include "I2C_prv.h"
#include "I2C_cfg.h"

static u8 MI2C1_u8WaitFlag(volatile u32 *A_pu32Reg, u32 A_u32BitNo)
{
    u32 L_u32Timeout = I2C_TIMEOUT_LOOPS;

    while(!GET_BIT(*A_pu32Reg, A_u32BitNo))
    {
        /* Check if a NACK (Acknowledge Failure) occurred */
        if(GET_BIT(I2C1->SR1, I2C_SR1_AF))
        {
            CLR_BIT(I2C1->SR1, I2C_SR1_AF); // Clear AF flag
            return I2C_ERR_NACK;
        }

        if(--L_u32Timeout == 0U)
        {
            return I2C_ERR_TIMEOUT;
        }
    }
    return I2C_OK;
}

void MI2C1_vInit(void)
{
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTB);
    MRCC_vEnableCLK(RCC_APB1, RCC_I2C1_BIT);

    GPIOx_PinConfig_t SCL =
    {
    	.Port = GPIO_PORTB,
		.Pin = GPIO_PIN6,
		.Mode = GPIO_MODE_AF,
		.OutputType = GPIO_OUTPUT_OPENDRAIN,
		.PullType = GPIO_PULL_UP,
		.Speed = GPIO_SPEED_VHIGH,
		.AltFunc = GPIO_AF4
    };

    GPIOx_PinConfig_t SDA =
    {
    	.Port = GPIO_PORTB,
		.Pin = GPIO_PIN7,
		.Mode = GPIO_MODE_AF,
		.OutputType = GPIO_OUTPUT_OPENDRAIN,
		.PullType = GPIO_PULL_UP,
		.Speed = GPIO_SPEED_VHIGH,
		.AltFunc = GPIO_AF4
    };

    MGPIO_vPinInit(&SCL);
    MGPIO_vPinInit(&SDA);

    SET_BIT(I2C1 -> CR1, I2C_CR1_SWRST);
    CLR_BIT(I2C1 -> CR1, I2C_CR1_SWRST);

    I2C1 -> CR2 &= ~(I2C_CR2_FREQ_MASK);
    I2C1 -> CR2 |= 16U;		//For HSI
    //I2C1 -> CR2 |= 25U;		//For HSE


    I2C1 -> CCR = 0x50;			//80 -> 100KHz  For HSI
    //I2C1 -> CCR = 0x7D;			//125 -> 100KHz For HSE

    I2C1 -> TRISE = 0x11;			// 17 For HSI
    //I2C1 -> TRISE = 0x1A;			// 26 For HSE

    SET_BIT(I2C1 -> CR1, I2C_CR1_PE);

}

u8 MI2C1_u8Start(void)
{
    SET_BIT(I2C1->CR1, I2C_CR1_START);
    return MI2C1_u8WaitFlag(&I2C1->SR1, I2C_SR1_SB);
}

void MI2C1_vStop(void)
{
    SET_BIT(I2C1->CR1, I2C_CR1_STOP);
}

u8 MI2C1_u8SendAddress(u8 A_u8DeviceAddress, u8 A_u8Read)
{
    u32 L_u32Timeout = I2C_TIMEOUT_LOOPS;

    /* Load Address into DR */
    I2C1->DR = (u32)((A_u8DeviceAddress << 1) | (A_u8Read ? 1U : 0U));

    while(!GET_BIT(I2C1->SR1, I2C_SR1_ADDR))
    {
        if(GET_BIT(I2C1->SR1, I2C_SR1_AF))
        {
            CLR_BIT(I2C1->SR1, I2C_SR1_AF);
            MI2C1_vStop();
            return I2C_ERR_NACK;
        }
        if(--L_u32Timeout == 0U)
        {
            return I2C_ERR_TIMEOUT;
        }
    }

    /* Clear ADDR Flag by reading SR1 followed by SR2 */
    (void)I2C1->SR1;
    (void)I2C1->SR2;

    return I2C_OK;
}

u8 MI2C1_u8SendByte(u8 A_u8Data)
{
    u8 L_u8Status = MI2C1_u8WaitFlag(&I2C1->SR1, I2C_SR1_TXE);
    if(L_u8Status != I2C_OK) return L_u8Status;

    I2C1->DR = A_u8Data;

    return MI2C1_u8WaitFlag(&I2C1->SR1, I2C_SR1_BTF);
}

u8 MI2C1_u8WriteReg(u8 A_u8DeviceAddress, u8 A_u8RegAddress, u8 A_u8Data)
{
    u8 L_u8Status;

    L_u8Status = MI2C1_u8Start();
    if(L_u8Status != I2C_OK) return L_u8Status;

    L_u8Status = MI2C1_u8SendAddress(A_u8DeviceAddress, 0U); // Write Mode
    if(L_u8Status != I2C_OK) return L_u8Status;

    L_u8Status = MI2C1_u8SendByte(A_u8RegAddress);
    if(L_u8Status != I2C_OK) { MI2C1_vStop(); return L_u8Status; }

    L_u8Status = MI2C1_u8SendByte(A_u8Data);
    MI2C1_vStop();

    return L_u8Status;
}

u8 MI2C1_u8ReadReg(u8 A_u8DeviceAddress, u8 A_u8RegAddress, u8* A_pu8Data)
{
    return MI2C1_u8ReadBurst(A_u8DeviceAddress, A_u8RegAddress, A_pu8Data, 1U);
}

u8 MI2C1_u8ReadBurst(u8 A_u8DeviceAddress, u8 A_u8RegAddress, u8* A_pu8Buffer, u16 A_u16Len)
{
    u8 L_u8Status;
    u16 L_u16Idx;
    u32 L_u32Timeout;

    if(A_pu8Buffer == NULL || A_u16Len == 0U) return I2C_ERR_NULL_PTR;

    /* Phase 1: Write Target Register Address */
    L_u8Status = MI2C1_u8Start();
    if(L_u8Status != I2C_OK) return L_u8Status;

    L_u8Status = MI2C1_u8SendAddress(A_u8DeviceAddress, 0U); // Write
    if(L_u8Status != I2C_OK) return L_u8Status;

    L_u8Status = MI2C1_u8SendByte(A_u8RegAddress);
    if(L_u8Status != I2C_OK) { MI2C1_vStop(); return L_u8Status; }

    /* Phase 2: Repeated START */
    L_u8Status = MI2C1_u8Start();
    if(L_u8Status != I2C_OK) return L_u8Status;

    /* Send Address Byte for Read Mode */
    I2C1->DR = (u32)((A_u8DeviceAddress << 1) | 1U);

    L_u32Timeout = I2C_TIMEOUT_LOOPS;
    while(!GET_BIT(I2C1->SR1, I2C_SR1_ADDR))
    {
        if(GET_BIT(I2C1->SR1, I2C_SR1_AF))
        {
            CLR_BIT(I2C1->SR1, I2C_SR1_AF);
            MI2C1_vStop();
            return I2C_ERR_NACK;
        }
        if(--L_u32Timeout == 0U) return I2C_ERR_TIMEOUT;
    }

    /* Phase 3: Hardware-Compliant Read Sequence */
    if (A_u16Len == 1U)
    {
        /* Disable ACK because this is the last byte */
        CLR_BIT(I2C1->CR1, I2C_CR1_ACK);

        /* Clear ADDR flag */
        (void)I2C1->SR1;
        (void)I2C1->SR2;

        /* Generate STOP */
        MI2C1_vStop();

        /* Wait until received byte is available */
        L_u32Timeout = I2C_TIMEOUT_LOOPS;

        while(!GET_BIT(I2C1->SR1, I2C_SR1_RXNE))
        {
            if(--L_u32Timeout == 0U)
            {
                return I2C_ERR_TIMEOUT;
            }
        }

        /* Read received byte */
        A_pu8Buffer[0] = (u8)I2C1->DR;
    }
    else
    {
        /* --- MULTI-BYTE BURST READ PROCEDURE --- */
        SET_BIT(I2C1->CR1, I2C_CR1_ACK);   // Ensure ACK is enabled

        /* Clear ADDR flag */
        (void)I2C1->SR1;
        (void)I2C1->SR2;

        for (L_u16Idx = 0U; L_u16Idx < A_u16Len; L_u16Idx++)
        {
            if (L_u16Idx == (A_u16Len - 2U))
            {
                /* When reading Byte (N-1), wait for RXNE, read byte, then disable ACK */
                L_u8Status = MI2C1_u8WaitFlag(&I2C1->SR1, I2C_SR1_RXNE);
                if(L_u8Status != I2C_OK) return L_u8Status;

                A_pu8Buffer[L_u16Idx] = (u8)I2C1->DR;

                CLR_BIT(I2C1->CR1, I2C_CR1_ACK); // Clear ACK for Nth byte
                MI2C1_vStop();                   // Set STOP condition
            }
            else if (L_u16Idx == (A_u16Len - 1U))
            {
                /* Read last byte */
                L_u8Status = MI2C1_u8WaitFlag(&I2C1->SR1, I2C_SR1_RXNE);
                if(L_u8Status != I2C_OK) return L_u8Status;

                A_pu8Buffer[L_u16Idx] = (u8)I2C1->DR;
            }
            else
            {
                /* Read intermediate bytes */
                L_u8Status = MI2C1_u8WaitFlag(&I2C1->SR1, I2C_SR1_RXNE);
                if(L_u8Status != I2C_OK) return L_u8Status;

                A_pu8Buffer[L_u16Idx] = (u8)I2C1->DR;
            }
        }
    }

    /* Restore ACK state for next transaction */
    SET_BIT(I2C1->CR1, I2C_CR1_ACK);

    return I2C_OK;
}
