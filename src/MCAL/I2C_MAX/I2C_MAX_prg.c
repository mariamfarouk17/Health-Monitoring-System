/*
 * I2C_MAX_prg.c
 *
 * Created on: Sep 7, 2026
 * Author: HP
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../RCC/RCC_int.h"
#include "../GPIO/GPIO_int.h"

#include "I2C_MAX_int.h"
#include "I2C_MAX_prv.h"
#include "I2C_MAX_cfg.h"


/* ============================================================
 * Debug variables
 * ============================================================ */

volatile u32 G_SR1_Debug    = 0U;
volatile u32 G_SR2_Debug    = 0U;
volatile u32 G_CR1_Debug    = 0U;
volatile u32 G_CR2_Debug    = 0U;

volatile u32 G_Address_SR1  = 0U;
volatile u32 G_Address_SR2  = 0U;
volatile u32 G_Address_DR   = 0U;

volatile u32 G_Phase        = 0U;
volatile u32 G_Phase_SR1   = 0U;
volatile u32 G_Phase_SR2   = 0U;


/* ============================================================
 * First transaction workaround
 * ============================================================ */

/*
 * 1 = workaround still needs to run
 * 0 = workaround already executed
 */
static u8 G_I2C_FirstTransaction = 1U;


/* ============================================================
 * Private functions
 * ============================================================ */

/*
 * Wait until a specific flag becomes set.
 */
static u8 MI2C2_MAX_u8WaitFlag(
    volatile u32 *A_pu32Reg,
    u32 A_u32BitNo)
{
    u32 L_u32Timeout = I2C_MAX_TIMEOUT_LOOPS;

    while (!GET_BIT(*A_pu32Reg, A_u32BitNo))
    {
        /*
         * Check for acknowledge failure.
         */
        if (GET_BIT(I2C2_MAX->SR1, I2C_MAX_SR1_AF))
        {
            /*
             * Clear AF.
             */
            I2C2_MAX->SR1 &=
                ~(1UL << I2C_MAX_SR1_AF);

            return I2C_MAX_ERR_NACK;
        }

        /*
         * Timeout protection.
         */
        if (--L_u32Timeout == 0U)
        {
            return I2C_MAX_ERR_TIMEOUT;
        }
    }

    return I2C_MAX_OK;
}


/*
 * ============================================================
 * Hidden first-transaction workaround
 * ============================================================
 *
 * Performs:
 *
 *     START
 *     Device Address + WRITE
 *     STOP
 *
 * exactly once after MI2C2_MAX_vInit().
 *
 * The address comes from the actual API call.
 */
static void MI2C2_MAX_vFirstTransactionWorkaround(
    u8 A_u8DeviceAddress)
{
    u8 L_u8Status;


    /*
     * Already executed?
     */
    if (G_I2C_FirstTransaction == 0U)
    {
        return;
    }


    /*
     * Mark consumed before executing.
     */
    G_I2C_FirstTransaction = 0U;


    /*
     * START
     */
    L_u8Status =
        MI2C2_MAX_u8Start();

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();
        return;
    }


    /*
     * Dummy WRITE address.
     *
     * Ignore the result intentionally.
     */
    (void)MI2C2_MAX_u8SendAddress(
        A_u8DeviceAddress,
        0U
    );


    /*
     * STOP
     */
    MI2C2_MAX_vStop();
}


/* ============================================================
 * I2C2 Initialization
 * ============================================================ */

void MI2C2_MAX_vInit(void)
{
    GPIOx_PinConfig_t SCL =
    {
        .Port       = GPIO_PORTB,
        .Pin        = GPIO_PIN10,
        .Mode       = GPIO_MODE_AF,
        .OutputType = GPIO_OUTPUT_OPENDRAIN,
        .PullType   = GPIO_PULL_UP,
        .Speed      = GPIO_SPEED_VHIGH,
        .AltFunc    = GPIO_AF4
    };


    GPIOx_PinConfig_t SDA =
    {
        .Port       = GPIO_PORTB,
        .Pin        = GPIO_PIN3,
        .Mode        = GPIO_MODE_AF,
        .OutputType = GPIO_OUTPUT_OPENDRAIN,
        .PullType   = GPIO_PULL_UP,
        .Speed      = GPIO_SPEED_VHIGH,
        .AltFunc    = GPIO_AF9
    };


    /*
     * Enable GPIOB clock.
     */
    MRCC_vEnableCLK(
        RCC_AHB1,
        GPIO_PORTB
    );


    /*
     * Enable I2C2 clock.
     */
    MRCC_vEnableCLK(
        RCC_APB1,
        RCC_I2C2_MAX_BIT
    );


    /*
     * Configure PB10 = SCL.
     * Configure PB3  = SDA.
     */
    MGPIO_vPinInit(&SCL);
    MGPIO_vPinInit(&SDA);


    /*
     * Reset I2C peripheral.
     */
    SET_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_SWRST
    );

    CLR_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_SWRST
    );


    /*
     * Configure I2C while disabled.
     */
    I2C2_MAX->CR1 = 0U;


    /*
     * PCLK1 = 16 MHz.
     */
    I2C2_MAX->CR2 =
        I2C_MAX_PCLK1_MHZ;


    /*
     * Own address.
     *
     * Bit 14 must be set in 7-bit addressing mode.
     */
    I2C2_MAX->OAR1 =
        0x4000U;


    /*
     * Standard mode, 100 kHz.
     *
     * CCR = 16 MHz / (2 * 100 kHz)
     *     = 80
     *     = 0x50
     */
    I2C2_MAX->CCR =
        0x50U;


    /*
     * Standard-mode TRISE.
     */
    I2C2_MAX->TRISE =
        I2C_MAX_PCLK1_MHZ + 1U;


    /*
     * Enable peripheral.
     */
    SET_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_PE
    );


    /*
     * Enable ACK.
     */
    SET_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_ACK
    );


    /*
     * Reset workaround state.
     */
    G_I2C_FirstTransaction = 1U;
}


/* ============================================================
 * Generate START
 * ============================================================ */

u8 MI2C2_MAX_u8Start(void)
{
    u32 L_u32Timeout =
        I2C_MAX_TIMEOUT_LOOPS;


    /*
     * Clear previous STOP request.
     */
    CLR_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_STOP
    );


    /*
     * Generate START.
     */
    SET_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_START
    );


    /*
     * Wait for SB.
     */
    while (
        !GET_BIT(
            I2C2_MAX->SR1,
            I2C_MAX_SR1_SB
        )
    )
    {
        if (--L_u32Timeout == 0U)
        {
            return I2C_MAX_ERR_TIMEOUT;
        }
    }


    return I2C_MAX_OK;
}


/* ============================================================
 * Generate STOP
 * ============================================================ */

void MI2C2_MAX_vStop(void)
{
    u32 L_u32Timeout =
        I2C_MAX_TIMEOUT_LOOPS;


    /*
     * Generate STOP.
     */
    SET_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_STOP
    );


    /*
     * SR2 BUSY = bit 1.
     */
    while (
        GET_BIT(
            I2C2_MAX->SR2,
            I2C_MAX_SR2_BUSY
        )
    )
    {
        if (--L_u32Timeout == 0U)
        {
            break;
        }
    }
}


/* ============================================================
 * Send slave address
 * ============================================================ */

u8 MI2C2_MAX_u8SendAddress(
    u8 A_u8DeviceAddress,
    u8 A_u8Read)
{
    u32 L_u32Timeout =
        I2C_MAX_TIMEOUT_LOOPS;

    u32 L_u32SR1;


    /*
     * Convert 7-bit address to address byte.
     *
     * Example:
     *
     * 0x57 WRITE = 0xAE
     * 0x57 READ  = 0xAF
     */
    I2C2_MAX->DR =
        (u32)(
            (A_u8DeviceAddress << 1U) |
            (A_u8Read ? 1U : 0U)
        );


    /*
     * Debug.
     */
    G_Address_DR =
        I2C2_MAX->DR;


    while (1)
    {
        L_u32SR1 =
            I2C2_MAX->SR1;


        /*
         * Address acknowledged.
         */
        if (
            GET_BIT(
                L_u32SR1,
                I2C_MAX_SR1_ADDR
            )
        )
        {
            /*
             * Clear ADDR:
             *
             * Read SR1 followed by SR2.
             */
            (void)I2C2_MAX->SR1;
            (void)I2C2_MAX->SR2;

            return I2C_MAX_OK;
        }


        /*
         * Address NACK.
         */
        if (
            GET_BIT(
                L_u32SR1,
                I2C_MAX_SR1_AF
            )
        )
        {
            /*
             * Save debug information.
             */
            G_Address_SR1 =
                L_u32SR1;

            G_Address_SR2 =
                I2C2_MAX->SR2;


            /*
             * Clear AF.
             */
            I2C2_MAX->SR1 &=
                ~(1UL << I2C_MAX_SR1_AF);


            /*
             * Abort transaction.
             */
            SET_BIT(
                I2C2_MAX->CR1,
                I2C_MAX_CR1_STOP
            );


            return I2C_MAX_ERR_NACK;
        }


        /*
         * Timeout.
         */
        if (--L_u32Timeout == 0U)
        {
            G_Address_SR1 =
                I2C2_MAX->SR1;

            G_Address_SR2 =
                I2C2_MAX->SR2;


            SET_BIT(
                I2C2_MAX->CR1,
                I2C_MAX_CR1_STOP
            );


            return I2C_MAX_ERR_TIMEOUT;
        }
    }
}


/* ============================================================
 * Send one byte
 * ============================================================ */

u8 MI2C2_MAX_u8SendByte(
    u8 A_u8Data)
{
    u8 L_u8Status;


    /*
     * Wait for TXE.
     */
    L_u8Status =
        MI2C2_MAX_u8WaitFlag(
            &I2C2_MAX->SR1,
            I2C_MAX_SR1_TXE
        );


    if (L_u8Status != I2C_MAX_OK)
    {
        return L_u8Status;
    }


    /*
     * Send byte.
     */
    I2C2_MAX->DR =
        A_u8Data;


    /*
     * Wait for BTF.
     */
    return MI2C2_MAX_u8WaitFlag(
        &I2C2_MAX->SR1,
        I2C_MAX_SR1_BTF
    );
}


/* ============================================================
 * Write register
 * ============================================================ */

u8 MI2C2_MAX_u8WriteReg(
    u8 A_u8DeviceAddress,
    u8 A_u8RegAddress,
    u8 A_u8Data)
{
    u8 L_u8Status;
    u32 L_u32Timeout;


    /*
     * Hidden first transaction.
     */
    MI2C2_MAX_vFirstTransactionWorkaround(
        A_u8DeviceAddress
    );


    /* =====================================================
     * START
     * ===================================================== */

    L_u8Status =
        MI2C2_MAX_u8Start();

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /* =====================================================
     * Device address + WRITE
     * ===================================================== */

    L_u8Status =
        MI2C2_MAX_u8SendAddress(
            A_u8DeviceAddress,
            0U
        );

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /* =====================================================
     * Register address
     * ===================================================== */

    L_u8Status =
        MI2C2_MAX_u8SendByte(
            A_u8RegAddress
        );

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /* =====================================================
     * Data byte
     * ===================================================== */

    L_u32Timeout =
        I2C_MAX_TIMEOUT_LOOPS;


    /*
     * Wait until data register becomes empty.
     */
    while (
        !GET_BIT(
            I2C2_MAX->SR1,
            I2C_MAX_SR1_TXE
        )
    )
    {
        /*
         * Check NACK.
         */
        if (
            GET_BIT(
                I2C2_MAX->SR1,
                I2C_MAX_SR1_AF
            )
        )
        {
            I2C2_MAX->SR1 &=
                ~(1UL << I2C_MAX_SR1_AF);

            MI2C2_MAX_vStop();

            return I2C_MAX_ERR_NACK;
        }


        /*
         * Timeout.
         */
        if (--L_u32Timeout == 0U)
        {
            G_SR1_Debug =
                I2C2_MAX->SR1;

            G_SR2_Debug =
                I2C2_MAX->SR2;

            MI2C2_MAX_vStop();

            return I2C_MAX_ERR_TIMEOUT;
        }
    }


    /*
     * Send final data byte.
     */
    I2C2_MAX->DR =
        A_u8Data;


    /*
     * Wait until DR is empty.
     */
    L_u32Timeout =
        I2C_MAX_TIMEOUT_LOOPS;


    while (
        !GET_BIT(
            I2C2_MAX->SR1,
            I2C_MAX_SR1_TXE
        )
    )
    {
        /*
         * Check NACK.
         */
        if (
            GET_BIT(
                I2C2_MAX->SR1,
                I2C_MAX_SR1_AF
            )
        )
        {
            G_SR1_Debug =
                I2C2_MAX->SR1;

            G_SR2_Debug =
                I2C2_MAX->SR2;


            I2C2_MAX->SR1 &=
                ~(1UL << I2C_MAX_SR1_AF);


            MI2C2_MAX_vStop();

            return I2C_MAX_ERR_NACK;
        }


        /*
         * Timeout.
         */
        if (--L_u32Timeout == 0U)
        {
            G_SR1_Debug =
                I2C2_MAX->SR1;

            G_SR2_Debug =
                I2C2_MAX->SR2;

            MI2C2_MAX_vStop();

            return I2C_MAX_ERR_TIMEOUT;
        }
    }


    /* =====================================================
     * STOP
     * ===================================================== */

    MI2C2_MAX_vStop();


    return I2C_MAX_OK;
}


/* ============================================================
 * Read one register
 * ============================================================ */

u8 MI2C2_MAX_u8ReadReg(
    u8 A_u8DeviceAddress,
    u8 A_u8RegAddress,
    u8 *A_pu8Data)
{
    return MI2C2_MAX_u8ReadBurst(
        A_u8DeviceAddress,
        A_u8RegAddress,
        A_pu8Data,
        1U
    );
}


/* ============================================================
 * Read burst
 * ============================================================ */

u8 MI2C2_MAX_u8ReadBurst(
    u8 A_u8DeviceAddress,
    u8 A_u8RegAddress,
    u8 *A_pu8Buffer,
    u16 A_u16Len)
{
    u8 L_u8Status;
    u32 L_u32Timeout;
    u16 L_u16Index;


    /*
     * Validate buffer.
     */
    if (
        (A_pu8Buffer == NULL) ||
        (A_u16Len == 0U)
    )
    {
        return I2C_MAX_ERR_NULL_PTR;
    }


    /*
     * Hidden first-transaction workaround.
     */
    MI2C2_MAX_vFirstTransactionWorkaround(
        A_u8DeviceAddress
    );


    /*
     * Phase 1: START
     */
    G_Phase = 1U;

    L_u8Status =
        MI2C2_MAX_u8Start();

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /*
     * Phase 2: Device address + WRITE
     */
    G_Phase = 2U;

    L_u8Status =
        MI2C2_MAX_u8SendAddress(
            A_u8DeviceAddress,
            0U
        );

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /*
     * Phase 3: Register address
     */
    G_Phase = 3U;

    L_u8Status =
        MI2C2_MAX_u8SendByte(
            A_u8RegAddress
        );

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /*
     * Phase 4: REPEATED START
     */
    G_Phase = 4U;

    L_u8Status =
        MI2C2_MAX_u8Start();

    if (L_u8Status != I2C_MAX_OK)
    {
        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /*
     * Phase 5: Configure receive
     *
     * For 1-byte receive, disable ACK before
     * clearing ADDR.
     */
    G_Phase = 5U;

    if (A_u16Len == 1U)
    {
        CLR_BIT(
            I2C2_MAX->CR1,
            I2C_MAX_CR1_ACK
        );
    }
    else
    {
        SET_BIT(
            I2C2_MAX->CR1,
            I2C_MAX_CR1_ACK
        );
    }


    /*
     * Phase 6: Device address + READ
     */
    G_Phase = 6U;

    L_u8Status =
        MI2C2_MAX_u8SendAddress(
            A_u8DeviceAddress,
            1U
        );


    G_Phase_SR1 =
        I2C2_MAX->SR1;

    G_Phase_SR2 =
        I2C2_MAX->SR2;


    if (L_u8Status != I2C_MAX_OK)
    {
        SET_BIT(
            I2C2_MAX->CR1,
            I2C_MAX_CR1_ACK
        );

        MI2C2_MAX_vStop();

        return L_u8Status;
    }


    /*
     * Phase 7: Receive
     */
    G_Phase = 7U;


    /* ----------------------------------------------------
     * Single-Byte Reception
     * ---------------------------------------------------- */

    if (A_u16Len == 1U)
    {
        SET_BIT(
            I2C2_MAX->CR1,
            I2C_MAX_CR1_STOP
        );


        L_u32Timeout =
            I2C_MAX_TIMEOUT_LOOPS;


        while (
            !GET_BIT(
                I2C2_MAX->SR1,
                I2C_MAX_SR1_RXNE
            )
        )
        {
            if (--L_u32Timeout == 0U)
            {
                SET_BIT(
                    I2C2_MAX->CR1,
                    I2C_MAX_CR1_ACK
                );

                return I2C_MAX_ERR_TIMEOUT;
            }
        }


        A_pu8Buffer[0] =
            (u8)I2C2_MAX->DR;


        SET_BIT(
            I2C2_MAX->CR1,
            I2C_MAX_CR1_ACK
        );


        return I2C_MAX_OK;
    }


    /* ----------------------------------------------------
     * Multi-Byte Reception
     * ---------------------------------------------------- */

    for (
        L_u16Index = 0U;
        L_u16Index < A_u16Len;
        L_u16Index++
    )
    {
        /*
         * Last byte:
         *
         * Disable ACK and request STOP.
         */
        if (
            L_u16Index ==
            (A_u16Len - 1U)
        )
        {
            CLR_BIT(
                I2C2_MAX->CR1,
                I2C_MAX_CR1_ACK
            );

            SET_BIT(
                I2C2_MAX->CR1,
                I2C_MAX_CR1_STOP
            );
        }


        L_u32Timeout =
            I2C_MAX_TIMEOUT_LOOPS;


        while (
            !GET_BIT(
                I2C2_MAX->SR1,
                I2C_MAX_SR1_RXNE
            )
        )
        {
            if (--L_u32Timeout == 0U)
            {
                SET_BIT(
                    I2C2_MAX->CR1,
                    I2C_MAX_CR1_ACK
                );

                return I2C_MAX_ERR_TIMEOUT;
            }
        }


        A_pu8Buffer[L_u16Index] =
            (u8)I2C2_MAX->DR;
    }


    /*
     * Restore ACK for future transactions.
     */
    SET_BIT(
        I2C2_MAX->CR1,
        I2C_MAX_CR1_ACK
    );


    return I2C_MAX_OK;
}


/* ============================================================
 * Debug getters
 * ============================================================ */

void MI2C2_MAX_vGetDebugRegisters(
    u32 *A_pu32SR1,
    u32 *A_pu32SR2,
    u32 *A_pu32CR1,
    u32 *A_pu32CR2)
{
    if (A_pu32SR1 != NULL)
    {
        *A_pu32SR1 =
            I2C2_MAX->SR1;
    }


    if (A_pu32SR2 != NULL)
    {
        *A_pu32SR2 =
            I2C2_MAX->SR2;
    }


    if (A_pu32CR1 != NULL)
    {
        *A_pu32CR1 =
            I2C2_MAX->CR1;
    }


    if (A_pu32CR2 != NULL)
    {
        *A_pu32CR2 =
            I2C2_MAX->CR2;
    }
}


/* ============================================================
 * Debug getters after init
 * ============================================================ */

void MI2C2_MAX_vGetDebugRegistersAfterInit(
    u32 *A_pu32SR1,
    u32 *A_pu32SR2,
    u32 *A_pu32CR1,
    u32 *A_pu32CR2)
{
    if (A_pu32SR1 != NULL)
    {
        *A_pu32SR1 =
            I2C2_MAX->SR1;
    }


    if (A_pu32SR2 != NULL)
    {
        *A_pu32SR2 =
            I2C2_MAX->SR2;
    }


    if (A_pu32CR1 != NULL)
    {
        *A_pu32CR1 =
            I2C2_MAX->CR1;
    }


    if (A_pu32CR2 != NULL)
    {
        *A_pu32CR2 =
            I2C2_MAX->CR2;
    }
}
