/*
 * VITALS_int.h
 *
 * Created on: Sep 14, 2026
 * Author: HP
 */

#ifndef APP_VITALS_VITALS_INT_H_
#define APP_VITALS_VITALS_INT_H_

#include "../../LIB/STD_TYPES.h"


/* =========================================================
 * VITALS
 * ========================================================= */

void MVITALS_vInit(void);


void MVITALS_vAddSample(
    u32 A_u32Red,
    u32 A_u32Ir
);


u8 MVITALS_u8IsBufferFull(void);


/* =========================================================
 * Heart Rate
 * ========================================================= */

u8 MVITALS_u8CalculateHR(
    u16 *A_pu16HR
);


/* =========================================================
 * SpO2
 * ========================================================= */

u8 MVITALS_u8CalculateSpO2(
    u8 *A_pu8SpO2
);


#endif /* APP_VITALS_VITALS_INT_H_ */
