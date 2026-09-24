/*
 * USART_prv.h
 *
 *  Created on: Aug 31, 2026
 *      Author: HP
 */

#ifndef MCAL_USART_USART_PRV_H_
#define MCAL_USART_USART_PRV_H_

#define USART1_BASE_ADDR       0x40011000U
#define USART2_BASE_ADDR       0x40004400U
#define USART6_BASE_ADDR       0x40011400U

typedef struct{
	u32 USARTx_SR;
	u32 USARTx_DR;
	u32 USARTx_BRR;
	u32 USARTx_CR1;
	u32 USARTx_CR2;
	u32 USARTx_CR3;
	u32 USARTx_GTPR;
}USARTx_MEM_MAP_t;

#define USART1          ((volatile USARTx_MEM_MAP_t *)(USART1_BASE_ADDR))
#define USART2          ((volatile USARTx_MEM_MAP_t *)(USART2_BASE_ADDR))
#define USART6          ((volatile USARTx_MEM_MAP_t *)(USART6_BASE_ADDR))

#endif /* MCAL_USART_USART_PRV_H_ */
