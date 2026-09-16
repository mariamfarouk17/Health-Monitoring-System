/*
 * main.c
 *
 * TEMPORARY TEST APPLICATION
 * Purpose: Hardware/driver test for the 7-Segment HAL ONLY.
 * Not part of the final application logic.
 *
 *  Created on: Sep 8, 2026
 *      Author: PC
 */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../HAL/S2P/S2P_int.h"
#include "../HAL/7SEG/7SEG_int.h"

int main(void)
{
    /* ---- TEST 1: Initialization ---- */
    MRCC_vInit();

    /* MODIFIED FOR M4: GPIOB is on AHB1 Bus in M4 architecture */
    MRCC_vEnableclk(RCC_AHB1, RCC_GPIOB);   /* PB13/PB14/PB15 = SEG_DATA/CLK/LATCH */

    H7SEG_vInit();                          /* Inits internal S2P instance + clears display */

    while (1)
    {
        /* ---- TEST 2: Display digits 0 to 9 sequentially ---- */
        for (u8 L_u8Digit = 0U; L_u8Digit <= 9U; L_u8Digit++)
        {
            H7SEG_vDisplayDigit(L_u8Digit);
            MSYSTICK_vSetDelay_us(100000U); /* ~1 second */
        }

        /* ---- TEST 3: Multi-digit numbers (driver supports 0-99) ---- */
        H7SEG_vDisplayNumber(12);
        MSYSTICK_vSetDelay_us(100000U);

        H7SEG_vDisplayNumber(25);
        MSYSTICK_vSetDelay_us(100000U);

        H7SEG_vDisplayNumber(50);
        MSYSTICK_vSetDelay_us(100000U);

        H7SEG_vDisplayNumber(78);
        MSYSTICK_vSetDelay_us(100000U);

    	H7SEG_vDisplayNumber(99);
        MSYSTICK_vSetDelay_us(100000U);

        /* ---- TEST 4: Clear function ---- */
        H7SEG_vDisplayNumber(42);
        MSYSTICK_vSetDelay_us(100000U);

     	H7SEG_vClear();
        MSYSTICK_vSetDelay_us(100000U);

        H7SEG_vDisplayNumber(7);
        MSYSTICK_vSetDelay_us(100000U);

        H7SEG_vClear();
        MSYSTICK_vSetDelay_us(100000U);
    }

    return 0;
}



