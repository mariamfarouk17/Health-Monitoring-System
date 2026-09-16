/*
 * 7SEG_prg.c
 *
 *  Created on: Sep 11, 2026
 *      Author: PC
 */




#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../S2P/S2P_int.h"
#include "../S2P/S2P_cfg.h"
#include "7SEG_int.h"
#include "7SEG_cfg.h"

/* Dedicated S2P handle owned by this HAL module (not exposed outside). */
static S2P_Init_t G_xSevenSegS2P;

/* Canonical "segment ON = 1" patterns for digits 0-9.
 * Polarity inversion (Common Anode/Cathode) is applied separately
 * in H7SEG_u8GetPattern() based on SEVEN_SEG_ACTIVE_HIGH. */
static const u8 G_au8SevenSegLUT[10] =
{
    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C) |
    (1U << SEVEN_SEG_BIT_D) | (1U << SEVEN_SEG_BIT_E) | (1U << SEVEN_SEG_BIT_F),                          /* 0 */

    (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C),                                                    /* 1 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_D) |
    (1U << SEVEN_SEG_BIT_E) | (1U << SEVEN_SEG_BIT_G),                                                    /* 2 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C) |
    (1U << SEVEN_SEG_BIT_D) | (1U << SEVEN_SEG_BIT_G),                                                    /* 3 */

    (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C) | (1U << SEVEN_SEG_BIT_F) |
    (1U << SEVEN_SEG_BIT_G),                                                                              /* 4 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_C) | (1U << SEVEN_SEG_BIT_D) |
    (1U << SEVEN_SEG_BIT_F) | (1U << SEVEN_SEG_BIT_G),                                                    /* 5 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_C) | (1U << SEVEN_SEG_BIT_D) |
    (1U << SEVEN_SEG_BIT_E) | (1U << SEVEN_SEG_BIT_F) | (1U << SEVEN_SEG_BIT_G),                          /* 6 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C),                          /* 7 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C) |
    (1U << SEVEN_SEG_BIT_D) | (1U << SEVEN_SEG_BIT_E) | (1U << SEVEN_SEG_BIT_F) | (1U << SEVEN_SEG_BIT_G),/* 8 */

    (1U << SEVEN_SEG_BIT_A) | (1U << SEVEN_SEG_BIT_B) | (1U << SEVEN_SEG_BIT_C) |
    (1U << SEVEN_SEG_BIT_D) | (1U << SEVEN_SEG_BIT_F) | (1U << SEVEN_SEG_BIT_G)                           /* 9 */
};

/* Applies the LUT and, if the hardware is inverted (SEVEN_SEG_ACTIVE_HIGH=0),
 * flips the polarity so the returned byte is exactly what must be shifted out. */
static u8 H7SEG_u8GetPattern(u8 Copy_u8Digit)
{
    u8 L_u8Pattern = G_au8SevenSegLUT[Copy_u8Digit];

#if (SEVEN_SEG_ACTIVE_HIGH == 0U)
    L_u8Pattern = (u8)(~L_u8Pattern);
#endif

    return L_u8Pattern;
}

void H7SEG_vInit(void)
{
    G_xSevenSegS2P.DataPort     = SEVEN_SEG_DATA_PORT;
    G_xSevenSegS2P.DataPin      = SEVEN_SEG_DATA_PIN;
    G_xSevenSegS2P.ShiftCLKPort = SEVEN_SEG_SHIFT_CLK_PORT;
    G_xSevenSegS2P.ShiftCLKPin  = SEVEN_SEG_SHIFT_CLK_PIN;
    G_xSevenSegS2P.LatchCLKPort = SEVEN_SEG_LATCH_CLK_PORT;
    G_xSevenSegS2P.LatchCLKPin  = SEVEN_SEG_LATCH_CLK_PIN;

    HS2P_vInit(&G_xSevenSegS2P);

    H7SEG_vClear();
}

void H7SEG_vDisplayDigit(u8 Copy_u8Digit)
{
    u8  L_u8Pattern;
    u32 L_u32Word;

    if (Copy_u8Digit > 9U)
    {
        return; /* invalid input - ignored */
    }

    L_u8Pattern = H7SEG_u8GetPattern(Copy_u8Digit);
    L_u32Word   = ((u32)L_u8Pattern) << SEVEN_SEG_UNITS_BIT_OFFSET;

    HS2P_vSendData(&G_xSevenSegS2P, L_u32Word);
}

void H7SEG_vDisplayNumber(u8 Copy_u8Number)
{
    u8  L_u8Units;
    u8  L_u8Tens;
    u8  L_u8UnitsPattern;
    u8  L_u8TensPattern;
    u32 L_u32Word;

    if (Copy_u8Number > 99U)
    {
        return; /* invalid input - ignored, only 2 digits available */
    }

    L_u8Units = Copy_u8Number % 10U;
    L_u8Tens  = (Copy_u8Number / 10U) % 10U;

    L_u8UnitsPattern = H7SEG_u8GetPattern(L_u8Units);
    L_u8TensPattern  = H7SEG_u8GetPattern(L_u8Tens);

    L_u32Word  = ((u32)L_u8UnitsPattern) << SEVEN_SEG_UNITS_BIT_OFFSET;
    L_u32Word |= ((u32)L_u8TensPattern)  << SEVEN_SEG_TENS_BIT_OFFSET;

    HS2P_vSendData(&G_xSevenSegS2P, L_u32Word);
}

void H7SEG_vClear(void)
{
    u32 L_u32Word;

#if (SEVEN_SEG_ACTIVE_HIGH == 1U)
    L_u32Word = 0x00000000UL; /* all segment bits = 0 -> all OFF */
#else
    L_u32Word = 0xFFFFFFFFUL; /* all segment bits = 1 -> all OFF (inverted) */
#endif

    HS2P_vSendData(&G_xSevenSegS2P, L_u32Word);
}
