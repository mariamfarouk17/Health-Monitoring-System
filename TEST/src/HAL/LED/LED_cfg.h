/*
 * LED_cfg.h
 *
 *  Created on: Sep 12, 2026
 *      Author: PC
 */
#ifndef HAL_LED_LED_CFG_H_
#define HAL_LED_LED_CFG_H_

/* =====================================================================
 * LED identifiers
 * ===================================================================== */
typedef enum
{
    LED_WIFI = 0,
    LED_ALERT,
    LED_BATTERY,
    LED_TOTAL   /* must stay last - used internally for bounds checking */
} LED_ID_t;

/* =====================================================================
 * LED active state
 * ===================================================================== */
#define LED_ACTIVE_HIGH     0U
#define LED_ACTIVE_LOW      1U

/* =====================================================================
 * Per-LED hardware configuration
 * ===================================================================== */
typedef struct
{
    u8 Port;
    u8 Pin;
    u8 ActiveState;
} LED_Cfg_t;

#define LED_CONFIG                                             \
{                                                              \
    /* LED_WIFI    */ { GPIO_PORTB, GPIO_PIN10, LED_ACTIVE_HIGH }, \
    /* LED_ALERT   */ { GPIO_PORTB, GPIO_PIN11, LED_ACTIVE_HIGH }, \
    /* LED_BATTERY */ { GPIO_PORTB, GPIO_PIN12, LED_ACTIVE_HIGH }  \
}

#endif /* HAL_LED_LED_CFG_H_ */
