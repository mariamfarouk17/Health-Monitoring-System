/*
 * MAX30102_int.h
 *
 *  Created on: Sep 13, 2026
 *      Author: HP
 */

#ifndef HAL_MAX30102_MAX30102_INT_H_
#define HAL_MAX30102_MAX30102_INT_H_

typedef struct
{
    u32 Red;
    u32 Ir;
} MAX30102_Sample_t;


/* Public API Prototypes */

u8 HMAX30102_u8Init(void);

u8 HMAX30102_u8ReadFifoSample(
    MAX30102_Sample_t *A_pxSample
);

u8 HMAX30102_u8ReadFifoBurst(
    MAX30102_Sample_t *A_pxBuffer,
    u8 A_u8NumSamples
);

void HMAX30102_vSetInterruptCallback(
    void (*A_pvFptr)(void)
);

u8 HMAX30102_u8ProcessInterrupt(
    MAX30102_Sample_t *A_pxSample
);


#endif /* HAL_MAX30102_MAX30102_INT_H_ */
