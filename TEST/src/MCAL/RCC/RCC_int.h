/*
 * RCC_int.h
 *
 *  Created on: Aug 17, 2026
 *      Author: PC
 */

#ifndef MCAL_RCC_RCC_INT_H_
#define MCAL_RCC_RCC_INT_H_


#define RCC_AHB1   0
#define RCC_AHB2   1
#define RCC_APB1   2
#define RCC_APB2   3



#define RCC_GPIOA 0
#define RCC_GPIOB 1
#define RCC_GPIOC 2
#define RCC_GPIOD 3



void MRCC_vInit(void);


void MRCC_vEnableclk(u8 A_u8BusID , u8 A_u8PeripheralID);


void MRCC_vDisableclk(u8 A_u8BusID , u8 A_u8PeripheralID);






#endif /* MCAL_RCC_RCC_INT_H_ */
