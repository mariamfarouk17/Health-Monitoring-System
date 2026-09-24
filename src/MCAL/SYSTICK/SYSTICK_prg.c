/*
 * SYSTICK_prg.c
 *
 *  Created on: Aug 24, 2026
 *      Author: Tona
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "SYSTICK_int.h"
#include "SYSTICK_prv.h"


/* =========================================================
 * SysTick timing
 *
 * F_CPU = 16 MHz
 * Clock source = AHB/8
 *
 * SysTick clock = 16 MHz / 8 = 2 MHz
 *
 * 1 ms = 2000 ticks
 * 1 us = 2 ticks
 * ========================================================= */

#define SYSTICK_CLOCK_HZ       2000000UL
#define SYSTICK_TICKS_PER_MS   2000.0
#define SYSTICK_TICKS_PER_US   2.0


/* =========================================================
 * Global callback
 * ========================================================= */

static void(*G_xFptr)(void) = NULL;

static u8 G_u8SingleFlag = 0U;


/* =========================================================
 * Initialization
 * ========================================================= */

void MSYSTICK_vInit(
    MSYSTICK_Config_t *A_xCfg
)
{
    /*
     * Stop SysTick
     */
    CLR_BIT(
        SYSTICK->CTRL,
        ENABLE
    );


    /*
     * Interrupt enable
     */
    if(
        A_xCfg->InterruptEnable ==
        INT_ENABLE
    )
    {
        SET_BIT(
            SYSTICK->CTRL,
            TICKINT
        );
    }

    else if(
        A_xCfg->InterruptEnable ==
        INT_DISABLE
    )
    {
        CLR_BIT(
            SYSTICK->CTRL,
            TICKINT
        );
    }


    /*
     * Clock source
     */
    if(
        A_xCfg->CLK_SRC ==
        CLK_SRC_AHB
    )
    {
        SET_BIT(
            SYSTICK->CTRL,
            CLKSOURCE
        );
    }

    else if(
        A_xCfg->CLK_SRC ==
        CLK_SRC_AHB_8
    )
    {
        CLR_BIT(
            SYSTICK->CTRL,
            CLKSOURCE
        );
    }
}


/* =========================================================
 * Start Timer
 * ========================================================= */

void MSYSTICK_vStartTimer(
    u32 A_u32LoadValue
)
{
    SYSTICK->LOAD =
        A_u32LoadValue;

    SYSTICK->VAL = 0U;

    SET_BIT(
        SYSTICK->CTRL,
        ENABLE
    );
}


/* =========================================================
 * Stop Timer
 * ========================================================= */

void MSYSTICK_vStopTimer(void)
{
    CLR_BIT(
        SYSTICK->CTRL,
        ENABLE
    );

    SYSTICK->VAL = 0U;
}


/* =========================================================
 * Get elapsed time
 * ========================================================= */

u32 MSYSTICK_u32GetElapsedTime_SingleShot(void)
{
    return(
        SYSTICK->LOAD -
        SYSTICK->VAL
    );
}


/* =========================================================
 * Get remaining time
 * ========================================================= */

u32 MSYSTICK_u32GetRemainingTime_SingleShot(void)
{
    return(
        SYSTICK->VAL
    );
}


/* =========================================================
 * Delay ms
 * ========================================================= */

void MSYSTICK_vSetDelay_ms(
    f64 A_f64Delay_ms
)
{
    u32 L_u32Ticks;


    /*
     * 2 MHz SysTick
     *
     * 1 ms = 2000 ticks
     */
    L_u32Ticks =
        (u32)(
            A_f64Delay_ms *
            SYSTICK_TICKS_PER_MS
        );


    SYSTICK->VAL = 0U;


    if(
        (L_u32Ticks >= 1U) &&
        (L_u32Ticks < 0x00FFFFFFUL)
    )
    {
        MSYSTICK_vStartTimer(
            L_u32Ticks
        );


        while(
            !GET_BIT(
                SYSTICK->CTRL,
                COUNTFLAG
            )
        )
        {
        }


        MSYSTICK_vStopTimer();
    }
}


/* =========================================================
 * Delay us
 * ========================================================= */

void MSYSTICK_vSetDelay_us(
    f64 A_f64Delay_us
)
{
    u32 L_u32Ticks;


    /*
     * 2 MHz SysTick
     *
     * 1 us = 2 ticks
     */
    L_u32Ticks =
        (u32)(
            A_f64Delay_us *
            SYSTICK_TICKS_PER_US
        );


    SYSTICK->VAL = 0U;


    if(
        (L_u32Ticks >= 1U) &&
        (L_u32Ticks < 0x00FFFFFFUL)
    )
    {
        MSYSTICK_vStartTimer(
            L_u32Ticks
        );


        while(
            !GET_BIT(
                SYSTICK->CTRL,
                COUNTFLAG
            )
        )
        {
        }


        MSYSTICK_vStopTimer();
    }
}


/* =========================================================
 * Single Interval
 * ========================================================= */

void MSYSTICK_vSetIntervalSingle(
    u32 A_u32Delay_ms,
    void (*Fptr)(void)
)
{
    u32 L_u32Ticks;


    G_u8SingleFlag = 1U;

    G_xFptr = Fptr;


    L_u32Ticks =
        (u32)(
            A_u32Delay_ms *
            SYSTICK_TICKS_PER_MS
        );


    SYSTICK->VAL = 0U;


    if(
        (L_u32Ticks >= 1U) &&
        (L_u32Ticks < 0x00FFFFFFUL)
    )
    {
        MSYSTICK_vStartTimer(
            L_u32Ticks
        );
    }
}


/* =========================================================
 * Multi Interval
 * ========================================================= */

void MSYSTICK_vSetIntervalMulti(
    u32 A_u32Delay_ms,
    void (*Fptr)(void)
)
{
    u32 L_u32Ticks;


    G_u8SingleFlag = 0U;

    G_xFptr = Fptr;


    L_u32Ticks =
        (u32)(
            A_u32Delay_ms *
            SYSTICK_TICKS_PER_MS
        );


    SYSTICK->VAL = 0U;


    if(
        (L_u32Ticks >= 1U) &&
        (L_u32Ticks < 0x00FFFFFFUL)
    )
    {
        MSYSTICK_vStartTimer(
            L_u32Ticks
        );
    }
}


/* =========================================================
 * Multi Interval - us
 * ========================================================= */

void MSYSTICK_vSetIntervalMulti_us(
    u32 A_u32Delay_us,
    void (*Fptr)(void)
)
{
    u32 L_u32Ticks;


    G_u8SingleFlag = 0U;

    G_xFptr = Fptr;


    L_u32Ticks =
        (u32)(
            A_u32Delay_us *
            SYSTICK_TICKS_PER_US
        );


    SYSTICK->VAL = 0U;


    if(
        (L_u32Ticks >= 1U) &&
        (L_u32Ticks < 0x00FFFFFFUL)
    )
    {
        MSYSTICK_vStartTimer(
            L_u32Ticks
        );
    }
}


/* =========================================================
 * SysTick Handler
 * ========================================================= */

void SysTick_Handler(void)
{
    if(
        G_xFptr != NULL
    )
    {
        G_xFptr();
    }


    if(
        G_u8SingleFlag == 1U
    )
    {
        MSYSTICK_vStopTimer();
    }
}
