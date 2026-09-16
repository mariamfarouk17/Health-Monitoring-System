/*
 * SYSCFG_prg.c
 *
 *  Created on: Aug 24, 2026
 *      Author: PC
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "SYSCFG_int.h"
#include "SYSCFG_prv.h"







void MSYSCFG_vSetLinePort(u8 A_u8LineNO , u8 A_u8Portnum)
{
	u8 Regnum = A_u8LineNO/4 ;
	u8 shift_value = (A_u8LineNO%4)*4;

	SYSCFG -> EXTICRx[Regnum] &= ~(0b1111 << shift_value);
	SYSCFG -> EXTICRx[Regnum] |= (A_u8Portnum << shift_value);
}
