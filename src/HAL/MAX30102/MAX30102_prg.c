/*
 * MAX30102_prg.c
 *
 * Created on: Sep 13, 2026
 * Author: HP
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/I2C_MAX/I2C_MAX_int.h"
#include "../../MCAL/SYSCFG/SYSCFG_int.h"
#include "../../MCAL/EXTI/EXTI_int.h"
#include "../../MCAL/NVIC/NVIC_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "MAX30102_cfg.h"
#include "MAX30102_prv.h"
#include "MAX30102_int.h"


volatile u8 G_u8MAXReadStatus = 0U;
volatile u8 G_u8MAXPartId = 0U;
volatile u8 G_u8MAXStep = 0U;

volatile u32 G_u32MAXInterruptCount = 0U;
volatile u8 G_u8MAXInterruptFlag = 0U;

static void (*G_MAX30102_Callback)(void) = NULL;


/* =========================================================
 * MAX30102 ISR
 * ========================================================= */

static void HMAX30102_vIsrInternal(void)
{
    G_u32MAXInterruptCount++;

    G_u8MAXInterruptFlag = 1U;
}


/* =========================================================
 * MAX30102 Init
 * ========================================================= */

u8 HMAX30102_u8Init(void)
{
    u8 L_u8Status;
    u8 L_u8PartId = 0U;


    G_u8MAXStep = 1U;


    /* =====================================================
     * Read Part ID
     * ===================================================== */

    L_u8Status =
        MI2C2_MAX_u8ReadReg(
            MAX30102_I2C_ADDR,
            MAX30102_REG_PART_ID,
            &L_u8PartId
        );


    G_u8MAXReadStatus = L_u8Status;
    G_u8MAXPartId = L_u8PartId;


    G_u8MAXStep = 2U;


    if(
        (L_u8Status != I2C_MAX_OK) ||
        (L_u8PartId != MAX30102_PART_ID_EXPECTED)
    )
    {
        G_u8MAXStep = 3U;

        return 1U;
    }


    G_u8MAXStep = 4U;


    /* =====================================================
     * Reset MAX30102
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_MODE_CONFIG,
        MAX30102_MODE_RESET_MASK
    );


    G_u8MAXStep = 5U;


    MSYSTICK_vSetDelay_ms(10U);


    G_u8MAXStep = 6U;


    /* =====================================================
     * MAX30102 INT Pin = PA0
     * ===================================================== */

    GPIOx_PinConfig_t IntPinCfg =
    {
        .Port       = MAX30102_INT_PORT,
        .Pin        = MAX30102_INT_PIN,
        .Mode       = GPIO_MODE_INPUT,
        .OutputType = GPIO_OUTPUT_PUSHPULL,
        .PullType   = GPIO_PULL_UP,
        .Speed      = GPIO_SPEED_LOW,
        .AltFunc    = 0U
    };


    MGPIO_vPinInit(
        &IntPinCfg
    );


    G_u8MAXStep = 7U;


    /* =====================================================
     * EXTI0 -> PA0
     * ===================================================== */

    MSYSCFG_vSetLinePort(
        MAX30102_INT_EXTI_LINE,
        MAX30102_INT_SYSCFG_PORT
    );


    G_u8MAXStep = 8U;


    MEXTI_vSetTrigger(
        MAX30102_INT_EXTI_LINE,
        EXTI_FALLING_EDGE
    );


    G_u8MAXStep = 9U;


    MEXTI_vSetCallBack(
        HMAX30102_vIsrInternal,
        MAX30102_INT_EXTI_LINE
    );


    G_u8MAXStep = 10U;


    MEXTI_vEnableINT(
        MAX30102_INT_EXTI_LINE
    );


    G_u8MAXStep = 11U;


    /* =====================================================
     * NVIC EXTI0
     * ===================================================== */

    MNVIC_vSetPeripheralPriority(
        MAX30102_INT_NVIC_IRQ,
        1U,
        0U
    );


    MNVIC_vEnable_Peripheral_INT(
        MAX30102_INT_NVIC_IRQ
    );


    G_u8MAXStep = 12U;


    /* =====================================================
     * Clear FIFO
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_FIFO_WR_PTR,
        0x00U
    );


    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_OVF_COUNTER,
        0x00U
    );


    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_FIFO_RD_PTR,
        0x00U
    );


    G_u8MAXStep = 13U;


    /* =====================================================
     * FIFO Configuration
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_FIFO_CONFIG,
        MAX30102_SAMPLE_AVG_4 |
        MAX30102_FIFO_ROLLOVER_EN |
        MAX30102_FIFO_A_FULL_4
    );


    G_u8MAXStep = 14U;


    /* =====================================================
     * SpO2 Mode
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_MODE_CONFIG,
        MAX30102_MODE_SPO2
    );


    G_u8MAXStep = 15U;


    /* =====================================================
     * SpO2 Configuration
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_SPO2_CONFIG,
        MAX30102_SPO2_ADC_4096 |
        MAX30102_SPO2_SR_100HZ |
        MAX30102_LED_PW_411US
    );


    G_u8MAXStep = 16U;


    /* =====================================================
     * RED LED Current
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_LED1_PA,
        MAX30102_LED_CURRENT_7_2MA
    );


    G_u8MAXStep = 17U;


    /* =====================================================
     * IR LED Current
     * ===================================================== */

    (void)MI2C2_MAX_u8WriteReg(
        MAX30102_I2C_ADDR,
        MAX30102_REG_LED2_PA,
        MAX30102_LED_CURRENT_7_2MA
    );


    G_u8MAXStep = 18U;


    /* =====================================================
     * IMPORTANT:
     *
     * Do NOT enable MAX30102 interrupt here.
     *
     * Main will:
     * 1. Wait for button
     * 2. Clear old interrupt status
     * 3. Clear EXTI pending
     * 4. Enable PPG_RDY
     *
     * This guarantees a clean interrupt start.
     * ===================================================== */

    G_u8MAXStep = 19U;


    return 0U;
}


/* =========================================================
 * Read One FIFO Sample
 * ========================================================= */

u8 HMAX30102_u8ReadFifoSample(
    MAX30102_Sample_t *A_pxSample
)
{
    u8 L_u8Buffer[6] = {0U};
    u8 L_u8Status;


    if(A_pxSample == NULL)
    {
        return 1U;
    }


    L_u8Status =
        MI2C2_MAX_u8ReadBurst(
            MAX30102_I2C_ADDR,
            MAX30102_REG_FIFO_DATA,
            L_u8Buffer,
            6U
        );


    if(L_u8Status != I2C_MAX_OK)
    {
        return L_u8Status;
    }


    A_pxSample->Red =
        (
            ((u32)L_u8Buffer[0] << 16U) |
            ((u32)L_u8Buffer[1] << 8U)  |
            ((u32)L_u8Buffer[2])
        ) & 0x0003FFFFU;


    A_pxSample->Ir =
        (
            ((u32)L_u8Buffer[3] << 16U) |
            ((u32)L_u8Buffer[4] << 8U)  |
            ((u32)L_u8Buffer[5])
        ) & 0x0003FFFFU;


    return I2C_MAX_OK;
}


/* =========================================================
 * Read FIFO Burst
 * ========================================================= */

u8 HMAX30102_u8ReadFifoBurst(
    MAX30102_Sample_t *A_pxBuffer,
    u8 A_u8NumSamples
)
{
    u8 L_u8Index;
    u8 L_u8Status = I2C_MAX_OK;


    if(
        (A_pxBuffer == NULL) ||
        (A_u8NumSamples == 0U)
    )
    {
        return 1U;
    }


    for(
        L_u8Index = 0U;
        L_u8Index < A_u8NumSamples;
        L_u8Index++
    )
    {
        L_u8Status =
            HMAX30102_u8ReadFifoSample(
                &A_pxBuffer[L_u8Index]
            );


        if(L_u8Status != I2C_MAX_OK)
        {
            break;
        }
    }


    return L_u8Status;
}


/* =========================================================
 * Process MAX30102 Interrupt
 * ========================================================= */

u8 HMAX30102_u8ProcessInterrupt(
    MAX30102_Sample_t *A_pxSample
)
{
    u8 L_u8Status;


    if(A_pxSample == NULL)
    {
        return 1U;
    }


    if(G_u8MAXInterruptFlag == 0U)
    {
        return 2U;
    }


    G_u8MAXInterruptFlag = 0U;


    L_u8Status =
        HMAX30102_u8ReadFifoSample(
            A_pxSample
        );


    return L_u8Status;
}


/* =========================================================
 * User Callback
 * ========================================================= */

void HMAX30102_vSetInterruptCallback(
    void (*A_pvFptr)(void)
)
{
    G_MAX30102_Callback = A_pvFptr;
}
