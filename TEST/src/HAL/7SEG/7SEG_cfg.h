/*
 * 7SEG_cfg.h
 *
 *  Created on: Sep 11, 2026
 *      Author: PC
 */

#ifndef HAL_7SEG_7SEG_CFG_H_
#define HAL_7SEG_7SEG_CFG_H_


/* =====================================================================
 * S2P pin mapping used by the 7-Segment display
 * (dedicated S2P instance for 7SEG only)
 * Confirmed from project pin map:
 *   PB13 -> SEG_DATA_PIN
 *   PB14 -> SEG_CLK_PIN
 *   PB15 -> SEG_LATCH_PIN
 * ===================================================================== */
#define SEVEN_SEG_DATA_PORT         GPIO_PORTB
#define SEVEN_SEG_DATA_PIN          GPIO_PIN13
#define SEVEN_SEG_SHIFT_CLK_PORT    GPIO_PORTB
#define SEVEN_SEG_SHIFT_CLK_PIN     GPIO_PIN14
#define SEVEN_SEG_LATCH_CLK_PORT    GPIO_PORTB
#define SEVEN_SEG_LATCH_CLK_PIN     GPIO_PIN15

/* =====================================================================
 * Display topology
 * ASSUMPTION (STILL NEEDS CONFIRMATION):
 * S2P_NO_OF_SHIFT_REG == 2  ->  two cascaded 8-bit shift registers,
 * each driving ONE full 7-segment digit (a,b,c,d,e,f,g,dp), statically
 * latched (no multiplexing/refresh).
 * ===================================================================== */
#define SEVEN_SEG_NO_OF_DIGITS       2U

/* Bit offset (inside the 16-bit word sent to HS2P_vSendData) of each digit.
 * Bits [7:0]  -> shift register wired directly to the MCU data pin (near).
 * Bits [15:8] -> second/far shift register in the chain.
 * TODO: confirm which physical digit (units/tens) sits on which register;
 * swap the two values below if your digits show swapped. */
#define SEVEN_SEG_UNITS_BIT_OFFSET   8U
#define SEVEN_SEG_TENS_BIT_OFFSET    0U

/* =====================================================================
 * Segment electrical polarity
 * TODO: CONFIRM against your board (Common Anode vs Common Cathode,
 * and whether the shift register output drives the segment directly
 * or through an inverting transistor stage).
 * 1 = writing bit '1' turns the segment ON
 * 0 = writing bit '1' turns the segment OFF (inverted output)
 * ===================================================================== */
#define SEVEN_SEG_ACTIVE_HIGH        1U

/* =====================================================================
 * Segment-to-bit mapping inside ONE digit's byte
 * TODO: CONFIRM against your board wiring / datasheet.
 * Default below follows the classic a..g,dp = bit0..bit7 convention,
 * taken from the commented reference code already in your main.c.
 * ===================================================================== */
#define SEVEN_SEG_BIT_A     0U
#define SEVEN_SEG_BIT_B     1U
#define SEVEN_SEG_BIT_C     2U
#define SEVEN_SEG_BIT_D     3U
#define SEVEN_SEG_BIT_E     4U
#define SEVEN_SEG_BIT_F     5U
#define SEVEN_SEG_BIT_G     6U
#define SEVEN_SEG_BIT_DP    7U



#endif /* HAL_7SEG_7SEG_CFG_H_ */
