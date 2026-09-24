/*
 * SYSTICK_prv.h
 *
 *  Created on: Aug 24, 2026
 *      Author: Tona
 */

#ifndef MCAL_SYSTICK_SYSTICK_PRV_H_
#define MCAL_SYSTICK_SYSTICK_PRV_H_

#define SYSTICK_BASE_ADDRESS	0xE000E010U

#define ENABLE			0
#define TICKINT			1
#define CLKSOURCE		2
#define COUNTFLAG		16


typedef struct
{
	u32 CTRL;
	u32 LOAD;
	u32 VAL;
	u32 CALIB;

}SYSTICK_MemMap_t;

#define SYSTICK 	((volatile SYSTICK_MemMap_t*)(SYSTICK_BASE_ADDRESS))

#endif /* MCAL_SYSTICK_SYSTICK_PRV_H_ */
