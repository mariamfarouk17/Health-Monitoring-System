/*
 * STK_int.h
 *
 *  Created on: Aug 24, 2026
 *      Author: HP
 */

#ifndef MCAL_STK_STK_INT_H_
#define MCAL_STK_STK_INT_H_

typedef struct{
	u8 InterruptEnable ;
	u8 CLK_SRC;
}MSTK_Config_t;

void MSTK_vInit(MSTK_Config_t *A_xCfg);

void MSTK_vStartTimer(u32 A_u32LoadValue);
void MSTK_vStopTimer(void);

u32 MSTK_u32GetElapsedTime_SingleShot(void);
u32 MSTK_u32GetRemainingTime_SingleShot(void);

void MSTK_vSetDelay_ms(u32 A_u32Delay_ms);
void MSTK_vSetDelay_us(u32 A_u32Delay_us);

void MSTK_vSetIntervalSingle(u32 A_u32Delay_ms , void (*Fptr)(void));
void MSTK_vSetIntervalMulti(u32 A_u32Delay_ms , void (*Fptr)(void));
void MSTK_vSetIntervalMulti_us(u32 A_u32Delay_us , void (*Fptr)(void));

// InterruptEnable
#define INT_ENABLE       1
#define INT_DISABLE      0

// CLK_SRC
#define CLK_SRC_AHB       1
#define CLK_SRC_AHB_8     2

#endif /* MCAL_STK_STK_INT_H_ */
