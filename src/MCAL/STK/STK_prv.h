/*
 * STK_prv.h
 *
 *  Created on: Aug 24, 2026
 *      Author: HP
 */

#ifndef MCAL_STK_STK_PRV_H_
#define MCAL_STK_STK_PRV_H_

#define STK_BASE_ADDR        0xE000E010U

typedef struct{
	u32 CTRL;
	u32 LOAD;
	u32 VAL;
	u32 CALIB;

}STK_MEM_MAP_t;

#define STK          ((volatile STK_MEM_MAP_t *)(STK_BASE_ADDR))

#define ENABLE       0
#define TICKINT      1
#define CLKSOURCE    2
#define COUNTFLAG    16

#endif /* MCAL_STK_STK_PRV_H_ */
