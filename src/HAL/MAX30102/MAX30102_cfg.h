/*
 * MAX30102_cfg.h
 *
 *  Created on: Sep 13, 2026
 *      Author: HP
 */

#ifndef HAL_MAX30102_MAX30102_CFG_H_
#define HAL_MAX30102_MAX30102_CFG_H_

/* I2C Address (7-bit address for MAX30102 is always 0x57) */
#define MAX30102_I2C_ADDR             0x57U

/* External Interrupt Pin Configuration */
#define MAX30102_INT_PORT             GPIO_PORTA
#define MAX30102_INT_PIN              GPIO_PIN0
#define MAX30102_INT_SYSCFG_PORT      SYSCFG_PORTA
#define MAX30102_INT_EXTI_LINE        EXTI_LINE0
#define MAX30102_INT_NVIC_IRQ         6U

/* Default Configuration Parameters */
#define MAX30102_SAMPLE_AVG_4         0x20U /* Average 4 samples per FIFO push */
#define MAX30102_FIFO_ROLLOVER_EN     0x10U /* Allow FIFO overflow to roll over */
#define MAX30102_FIFO_A_FULL_4        0x0FU /* Interrupt when 4 empty slots remain */

#define MAX30102_MODE_SPO2             0x03U /* Red + IR LED active */
#define MAX30102_SPO2_ADC_4096        0x20U /* Full Scale 4096nA, 15.8-bit resolution */
#define MAX30102_SPO2_SR_100HZ        0x04U /* 100 Samples Per Second */
#define MAX30102_LED_PW_411US         0x03U /* Pulse width 411us (18-bit ADC resolution) */

#define MAX30102_LED_CURRENT_7_2MA    0x24U /* LED Current = ~7.2 mA */

#endif /* HAL_MAX30102_MAX30102_CFG_H_ */
