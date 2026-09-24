/*
 * RCC_prv.h
 *
 *  Created on: Aug 17, 2026
 *      Author: HP
 */

#ifndef MCAL_RCC_RCC_PRV_H_
#define MCAL_RCC_RCC_PRV_H_

#define RCC_BASE_ADDR   0x40023800U

typedef struct{
	u32 CR ;
	u32 PLLCFGR;
	u32 CFGR;
	u32 CIR;
	u32 AHB1RSTR;
	u32 AHB2RSTR;

	u32 Reserved1;    // 0x18
	u32 Reserved2;    // 0x1C

	u32 APB1RSTR;
	u32 APB2RSTR;

	u32 Reserved3;    // 0x28
	u32 Reserved4;    // 0x2C

	u32 AHB1ENR;
	u32 AHB2ENR;

	u32 Reserved5;    // 0x38
	u32 Reserved6;    // 0x3C

	u32 APB1ENR;
	u32 APB2ENR;

	u32 Reserved7;    // 0x48
	u32 Reserved8;    // 0x4C

	u32 AHB1LPENR;
	u32 AHB2LPENR;

	u32 Reserved9;    // 0x58
	u32 Reserved10;   // 0x5C

	u32 APB1LPENR;
	u32 APB2LPENR;

	u32 Reserved11;   // 0x68
	u32 Reserved12;   // 0x6C

	u32 BDCR;
	u32 CSR;

	u32 Reserved13;   // 0x78
	u32 Reserved14;   // 0x7C

	u32 SSCGR;
	u32 PLLI2SCFGR;

	u32 Reserved15;   // 0x88

	u32 DCKCFGR;

}RCC_MEM_MAP_t;

#define RCC      ((volatile RCC_MEM_MAP_t *)(RCC_BASE_ADDR))

// SYSCLK OPTIONS
#define HSE_CLK   1
#define HSI_CLK   2
#define PLL_CLK   3

// BYPASS
#define RC_CLK           0
#define MECHANICAL_CLK   1

// HSE CONTROL
#define HSE_ON     16
#define HSE_RDY    17
#define HSE_BYP    18

// HSI CONTROL
#define HSI_ON     0
#define HSI_RDY    1
#endif /* MCAL_RCC_RCC_PRV_H_ */
