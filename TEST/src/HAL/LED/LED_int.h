/*
 * LED_int.h
 *
 *  Created on: Sep 12, 2026
 *      Author: PC
 */
#ifndef HAL_LED_LED_INT_H_
#define HAL_LED_LED_INT_H_

#include "LED_cfg.h"

/* Initializes all configured LEDs as outputs and forces them OFF. */
void HLED_vInit(void);

/* Turns the given LED physically ON. Invalid IDs are ignored. */
void HLED_vTurnOn(LED_ID_t Copy_xLedId);

/* Turns the given LED physically OFF. Invalid IDs are ignored. */
void HLED_vTurnOff(LED_ID_t Copy_xLedId);

/* Toggles the given LED's current physical state. Invalid IDs are ignored. */
void HLED_vToggle(LED_ID_t Copy_xLedId);

/* Returns the LED's logical state: GPIO_HIGH (ON) or GPIO_LOW (OFF).
 * Returns GPIO_LOW for an invalid ID. */
u8 HLED_u8GetState(LED_ID_t Copy_xLedId);

#endif /* HAL_LED_LED_INT_H_ */
