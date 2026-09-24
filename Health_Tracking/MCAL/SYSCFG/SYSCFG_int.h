/*
 * SYSCFG_int.h
 *
 *  Created on: Aug 24, 2026
 *      Author: Tona
 */

#ifndef MCAL_SYSCFG_SYSCFG_INT_H_
#define MCAL_SYSCFG_SYSCFG_INT_H_


#define SYSCFG_PORTA	0b0000
#define SYSCFG_PORTB	0b0001
#define SYSCFG_PORTC	0b0010

void MSYSCFG_vSetLinePort(u8 A_u8LineNo, u8 A_u8PortNo);

#endif /* MCAL_SYSCFG_SYSCFG_INT_H_ */
