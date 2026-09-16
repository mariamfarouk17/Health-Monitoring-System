/*
 * LED_prg.c
 *
 *  Created on: Sep 12, 2026
 *      Author: PC
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "LED_int.h"
#include "LED_cfg.h"

static const LED_Cfg_t G_axLedCfg[LED_TOTAL] = LED_CONFIG;

/* Converts a logical ON/OFF request into the raw pin level required,
 * taking the LED's configured polarity into account. */
static u8 HLED_u8LogicalToPinLevel(LED_ID_t Copy_xLedId, u8 Copy_u8LogicalOn)
{
    u8 L_u8PinLevel;

    if (G_axLedCfg[Copy_xLedId].ActiveState == LED_ACTIVE_HIGH)
    {
        L_u8PinLevel = (Copy_u8LogicalOn == GPIO_HIGH) ? GPIO_HIGH : GPIO_LOW;
    }
    else /* LED_ACTIVE_LOW */
    {
        L_u8PinLevel = (Copy_u8LogicalOn == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH;
    }

    return L_u8PinLevel;
}

void HLED_vInit(void)
{
    u8 L_u8Idx;
    GPIOx_PinConfig_t L_xPinCfg;

    for (L_u8Idx = 0U; L_u8Idx < (u8)LED_TOTAL; L_u8Idx++)
    {
        L_xPinCfg.Port       = G_axLedCfg[L_u8Idx].Port;
        L_xPinCfg.Pin        = G_axLedCfg[L_u8Idx].Pin;
        L_xPinCfg.Mode       = GPIO_MODE_OUTPUT;
        L_xPinCfg.OutputType = GPIO_OT_PUSHPULL;
        L_xPinCfg.Speed      = GPIO_SPEED_LOW;
        L_xPinCfg.PullType   = GPIO_NO_PULL;
        L_xPinCfg.AltFunc    = GPIO_AF0;

        MGPIO_vPinInit(&L_xPinCfg);

        /* Force OFF at init, regardless of polarity */
        MGPIO_vSetPinValue(G_axLedCfg[L_u8Idx].Port,
                           G_axLedCfg[L_u8Idx].Pin,
                           HLED_u8LogicalToPinLevel((LED_ID_t)L_u8Idx, GPIO_LOW));
    }
}

void HLED_vTurnOn(LED_ID_t Copy_xLedId)
{
    if (Copy_xLedId >= LED_TOTAL)
    {
        return; /* Invalid ID - ignored */
    }

    MGPIO_vSetPinValue(G_axLedCfg[Copy_xLedId].Port,
                       G_axLedCfg[Copy_xLedId].Pin,
                       HLED_u8LogicalToPinLevel(Copy_xLedId, GPIO_HIGH));
}

void HLED_vTurnOff(LED_ID_t Copy_xLedId)
{
    if (Copy_xLedId >= LED_TOTAL)
    {
        return; /* Invalid ID - ignored */
    }

    MGPIO_vSetPinValue(G_axLedCfg[Copy_xLedId].Port,
                       G_axLedCfg[Copy_xLedId].Pin,
                       HLED_u8LogicalToPinLevel(Copy_xLedId, GPIO_LOW));
}

void HLED_vToggle(LED_ID_t Copy_xLedId)
{
    if (Copy_xLedId >= LED_TOTAL)
    {
        return; /* Invalid ID - ignored */
    }

    MGPIO_vTogPinValue(G_axLedCfg[Copy_xLedId].Port,
                        G_axLedCfg[Copy_xLedId].Pin);
}

u8 HLED_u8GetState(LED_ID_t Copy_xLedId)
{
    u8 L_u8PinLevel;
    u8 L_u8LogicalState;

    if (Copy_xLedId >= LED_TOTAL)
    {
        return GPIO_LOW; /* Invalid ID - report OFF */
    }

    L_u8PinLevel = MGPIO_u8GetPinValue(G_axLedCfg[Copy_xLedId].Port,
                                        G_axLedCfg[Copy_xLedId].Pin);

    if (G_axLedCfg[Copy_xLedId].ActiveState == LED_ACTIVE_HIGH)
    {
        L_u8LogicalState = (L_u8PinLevel == GPIO_HIGH) ? GPIO_HIGH : GPIO_LOW;
    }
    else /* LED_ACTIVE_LOW */
    {
        L_u8LogicalState = (L_u8PinLevel == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH;
    }

    return L_u8LogicalState;
}
