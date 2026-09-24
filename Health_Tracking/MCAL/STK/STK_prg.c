/*
 * STK_prg.c
 *
 * Created on: Aug 24, 2026
 * Author: HP
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "STK_int.h"
#include "STK_prv.h"
#include "STK_cfg.h"


/* ============================================================
 * SysTick clock configuration
 *
 * HSI = 16 MHz
 * SysTick clock = HSI / 8 = 2 MHz
 *
 * 1 tick = 0.5 us
 * 1 us  = 2 ticks
 * 1 ms  = 2000 ticks
 * ============================================================ */

#define STK_CLOCK_HZ        2000000UL
#define STK_TICKS_PER_MS    2000UL
#define STK_TICKS_PER_US    2UL


static void (*G_xFptr)(void) = NULL;

static u8 G_u8SingleFlag = 0;


void MSTK_vInit(MSTK_Config_t *A_xCfg)
{
    /* Stop */
    CLR_BIT(STK->CTRL, ENABLE);

    if(A_xCfg->InterruptEnable == INT_ENABLE)
    {
        SET_BIT(STK->CTRL, TICKINT);
    }
    else if(A_xCfg->InterruptEnable == INT_DISABLE)
    {
        CLR_BIT(STK->CTRL, TICKINT);
    }

    if(A_xCfg->CLK_SRC == CLK_SRC_AHB)
    {
        SET_BIT(STK->CTRL, CLKSOURCE);
    }
    else if(A_xCfg->CLK_SRC == CLK_SRC_AHB_8)
    {
        CLR_BIT(STK->CTRL, CLKSOURCE);
    }
}


void MSTK_vStartTimer(u32 A_u32LoadValue)
{
    /* Load Value */
    STK->LOAD = A_u32LoadValue;

    /* Clear Current Counter Value */
    STK->VAL = 0;

    /* Start */
    SET_BIT(STK->CTRL, ENABLE);
}


void MSTK_vStopTimer(void)
{
    /* Stop SysTick */
    CLR_BIT(STK->CTRL, ENABLE);

    /* Clear Current Counter Value */
    STK->VAL = 0;
}


u32 MSTK_u32GetElapsedTime_SingleShot(void)
{
    u32 L_u32ElapsedTime = 0;

    L_u32ElapsedTime = STK->LOAD - STK->VAL;

    return L_u32ElapsedTime;
}


u32 MSTK_u32GetRemainingTime_SingleShot(void)
{
    u32 L_u32RemainingTime = 0;

    L_u32RemainingTime = STK->VAL;

    return L_u32RemainingTime;
}


void MSTK_vSetDelay_ms(u32 A_u32Delay_ms)
{
    u32 L_u32Ticks;
    u32 L_u32PreviousVAL;
    u32 L_u32CurrentVAL;

    L_u32Ticks = A_u32Delay_ms * STK_TICKS_PER_MS;

    if((L_u32Ticks >= 1U) &&
       (L_u32Ticks <= 0x00FFFFFFU))
    {
        MSTK_vStartTimer(L_u32Ticks - 1U);

        L_u32PreviousVAL = STK->VAL;

        while(1)
        {
            L_u32CurrentVAL = STK->VAL;

            if(L_u32CurrentVAL > L_u32PreviousVAL)
            {
                break;
            }

            L_u32PreviousVAL = L_u32CurrentVAL;
        }

        MSTK_vStopTimer();
    }
}


void MSTK_vSetDelay_us(u32 A_u32Delay_us)
{
    u32 L_u32Ticks;

    /*
     * HSI = 16 MHz
     * SysTick = 2 MHz
     *
     * 1 us = 2 ticks
     */

    L_u32Ticks = A_u32Delay_us * STK_TICKS_PER_US;

    if((L_u32Ticks >= 1U) &&
       (L_u32Ticks <= 0x00FFFFFFU))
    {
        /* Reset current counter */
        STK->VAL = 0;

        MSTK_vStartTimer(L_u32Ticks - 1U);

        /* Wait until timer reaches zero */
        while(!GET_BIT(STK->CTRL, COUNTFLAG));
    }
}


void MSTK_vSetIntervalSingle(
    u32 A_u32Delay_ms,
    void (*Fptr)(void))
{
    G_u8SingleFlag = 1;

    u32 L_u32Ticks =
        (u32)(A_u32Delay_ms * STK_TICKS_PER_MS);

    G_xFptr = Fptr;

    STK->VAL = 0;

    if((L_u32Ticks >= 1U) &&
       (L_u32Ticks <= 0x00FFFFFFU))
    {
        MSTK_vStartTimer(L_u32Ticks - 1U);
    }
}


void MSTK_vSetIntervalMulti(
    u32 A_u32Delay_ms,
    void (*Fptr)(void))
{
    G_u8SingleFlag = 0;

    u32 L_u32Ticks =
        (u32)(A_u32Delay_ms * STK_TICKS_PER_MS);

    G_xFptr = Fptr;

    STK->VAL = 0;

    if((L_u32Ticks >= 1U) &&
       (L_u32Ticks <= 0x00FFFFFFU))
    {
        MSTK_vStartTimer(L_u32Ticks - 1U);
    }
}


void MSTK_vSetIntervalMulti_us(
    u32 A_u32Delay_us,
    void (*Fptr)(void))
{
    G_u8SingleFlag = 0;

    u32 L_u32Ticks =
        (u32)(A_u32Delay_us * STK_TICKS_PER_US);

    G_xFptr = Fptr;

    STK->VAL = 0;

    if((L_u32Ticks >= 1U) &&
       (L_u32Ticks <= 0x00FFFFFFU))
    {
        MSTK_vStartTimer(L_u32Ticks - 1U);
    }
}


//void SysTick_Handler(void)
//{
//    if(G_xFptr != NULL)
//    {
//        G_xFptr();
//    }
//
//    if(G_u8SingleFlag == 1)
//    {
//        /* Single Mode */
//        MSTK_vStopTimer();
//    }
//}
