/*
 * TFT_int.h
 *
 *  Created on: Sep 3, 2026
 *      Author: ME
 */

#ifndef HAL_TFT_TFT_INT_H_
#define HAL_TFT_TFT_INT_H_

#define TFT_BLACK   0x0000
#define TFT_WHITE   0xFFFF
#define TFT_GREEN   0x07E0
#define TFT_RED     0xF800
#define TFT_BLUE 	0x023F5
#define TFT_GRAY	0x8410

void HTFT_vInit(void);

void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize);

void HTFT_vSetXPos(u16 A_u16xStart, u16 A_u16xEnd);
void HTFT_vSetYPos(u16 A_u16yStart, u16 A_u16yEnd);
void HTFT_vFillBackgroundColor(u16 A_u16Color);
void HTFT_vFillRectangle(u16 A_u16Color);

// Character and Text Functions
void HTFT_vDrawPixel(u16 A_u16X, u16 A_u16Y, u16 A_u16Color);
void HTFT_vPrintChar(u8 A_u8Char, u16 A_u16X, u16 A_u16Y, u16 A_u16Color, u16 A_u16BgColor);
void HTFT_vPrintString(const char* A_pu8String, u16 A_u16X, u16 A_u16Y, u16 A_u16Color, u16 A_u16BgColor);
void HTFT_vPrintNumber(f32 A_s32Number, u16 A_u16X, u16 A_u16Y, u16 A_u16Color, u16 A_u16BgColor);

// Graphics and Shape Functions
void HTFT_vDrawLine(u16 A_u16X0, u16 A_u16Y0, u16 A_u16X1, u16 A_u16Y1, u16 A_u16Color);
void HTFT_vDrawRect(u16 A_u16X, u16 A_u16Y, u16 A_u16Width, u16 A_u16Height, u16 A_u16Color);
void HTFT_vDrawSolidRect(u16 A_u16X, u16 A_u16Y, u16 A_u16Width, u16 A_u16Height, u16 A_u16Color);

//Interfacing Functions
void TFT_vDashBoard(void);
void TFT_vStepsReview(u16 steps, f32 dist, f32 calories, u8 activity);
void TFT_vSettings(void);
void TFT_vGraph(void);

#endif /* HAL_TFT_TFT_INT_H_ */
