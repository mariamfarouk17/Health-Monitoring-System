/*
 * 7SEG_int.h
 *
 *  Created on: Sep 11, 2026
 *      Author: PC
 */

#ifndef HAL_7SEG_7SEG_INT_H_
#define HAL_7SEG_7SEG_INT_H_


/* Initializes the underlying S2P instance used by the 7-segment display
 * and blanks the display. Must be called once before any other API. */
void H7SEG_vInit(void);

/* Displays a single digit (0-9) on the "units" position.
 * Invalid input (>9) is ignored (no change to the display). */
void H7SEG_vDisplayDigit(u8 Copy_u8Digit);

/* Displays a two-digit number (0-99) using both digit positions.
 * Invalid input (>99) is ignored (no change to the display). */
void H7SEG_vDisplayNumber(u8 Copy_u8Number);

/* Turns all segments off on all digits. */
void H7SEG_vClear(void);



#endif /* HAL_7SEG_7SEG_INT_H_ */
