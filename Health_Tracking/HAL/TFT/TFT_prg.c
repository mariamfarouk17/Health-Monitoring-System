/*
 * TFT_prg.c
 *
 *  Created on: Sep 3, 2026
 *      Author: ME
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SPI/SPI_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "TFT_int.h"

#define TFT_WIDTH 	128U
#define TFT_HEIGHT 	160U

#define TFT_BLACK   0x0000
#define TFT_WHITE   0xFFFF
#define TFT_GREEN   0x07E0
#define TFT_RED     0xF800
#define TFT_BLUE 	0x023F5
#define TFT_GRAY	0x8410

static u16 CurrentXStart;
static u16 CurrentYStart;

static u16 CurrentXEnd;
static u16 CurrentYEnd;

static const u8 Font5x8[128][5] =
{
    {0x00,0x00,0x00,0x00,0x00}, /* ' ' 0x20 */
    {0x00,0x00,0x5F,0x00,0x00}, /* '!' */
    {0x00,0x07,0x00,0x07,0x00}, /* '"' */
    {0x14,0x7F,0x14,0x7F,0x14}, /* '#' */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* '$' */
    {0x23,0x13,0x08,0x64,0x62}, /* '%' */
    {0x36,0x49,0x55,0x22,0x50}, /* '&' */
    {0x00,0x05,0x03,0x00,0x00}, /* ''' */
    {0x00,0x1C,0x22,0x41,0x00}, /* '(' */
    {0x00,0x41,0x22,0x1C,0x00}, /* ')' */
    {0x14,0x08,0x3E,0x08,0x14}, /* '*' */
    {0x08,0x08,0x3E,0x08,0x08}, /* '+' */
    {0x00,0x50,0x30,0x00,0x00}, /* ',' */
    {0x08,0x08,0x08,0x08,0x08}, /* '-' */
    {0x00,0x60,0x60,0x00,0x00}, /* '.' */
    {0x20,0x10,0x08,0x04,0x02}, /* '/' */
    {0x3E,0x51,0x49,0x45,0x3E}, /* '0' */
    {0x00,0x42,0x7F,0x40,0x00}, /* '1' */
    {0x42,0x61,0x51,0x49,0x46}, /* '2' */
    {0x21,0x41,0x45,0x4B,0x31}, /* '3' */
    {0x18,0x14,0x12,0x7F,0x10}, /* '4' */
    {0x27,0x45,0x45,0x45,0x39}, /* '5' */
    {0x3C,0x4A,0x49,0x49,0x30}, /* '6' */
    {0x01,0x71,0x09,0x05,0x03}, /* '7' */
    {0x36,0x49,0x49,0x49,0x36}, /* '8' */
    {0x06,0x49,0x49,0x29,0x1E}, /* '9' */
    {0x00,0x36,0x36,0x00,0x00}, /* ':' */
    {0x00,0x56,0x36,0x00,0x00}, /* ';' */
    {0x08,0x14,0x22,0x41,0x00}, /* '<' */
    {0x14,0x14,0x14,0x14,0x14}, /* '=' */
    {0x00,0x41,0x22,0x14,0x08}, /* '>' */
    {0x02,0x01,0x51,0x09,0x06}, /* '?' */
	{0x32,0x49,0x79,0x41,0x3E}, /* '@' */
    {0x7E,0x11,0x11,0x11,0x7E}, /* 'A' */
    {0x7F,0x49,0x49,0x49,0x36}, /* 'B' */
    {0x3E,0x41,0x41,0x41,0x22}, /* 'C' */
    {0x7F,0x41,0x41,0x22,0x1C}, /* 'D' */
    {0x7F,0x49,0x49,0x49,0x41}, /* 'E' */
    {0x7F,0x09,0x09,0x09,0x01}, /* 'F' */
    {0x3E,0x41,0x49,0x49,0x7A}, /* 'G' */
    {0x7F,0x08,0x08,0x08,0x7F}, /* 'H' */
    {0x00,0x41,0x7F,0x41,0x00}, /* 'I' */
    {0x20,0x40,0x41,0x3F,0x01}, /* 'J' */
    {0x7F,0x08,0x14,0x22,0x41}, /* 'K' */
    {0x7F,0x40,0x40,0x40,0x40}, /* 'L' */
    {0x7F,0x02,0x0C,0x02,0x7F}, /* 'M' */
    {0x7F,0x04,0x08,0x10,0x7F}, /* 'N' */
    {0x3E,0x41,0x41,0x41,0x3E}, /* 'O' */
    {0x7F,0x09,0x09,0x09,0x06}, /* 'P' */
    {0x3E,0x41,0x51,0x21,0x5E}, /* 'Q' */
    {0x7F,0x09,0x19,0x29,0x46}, /* 'R' */
    {0x46,0x49,0x49,0x49,0x31}, /* 'S' */
    {0x01,0x01,0x7F,0x01,0x01}, /* 'T' */
    {0x3F,0x40,0x40,0x40,0x3F}, /* 'U' */
    {0x1F,0x20,0x40,0x20,0x1F}, /* 'V' */
    {0x3F,0x40,0x38,0x40,0x3F}, /* 'W' */
    {0x63,0x14,0x08,0x14,0x63}, /* 'X' */
    {0x07,0x08,0x70,0x08,0x07}, /* 'Y' */
    {0x61,0x51,0x49,0x45,0x43}, /* 'Z' */
	{0x00,0x7F,0x41,0x41,0x00}, /* '[' */
    {0x02,0x04,0x08,0x10,0x20}, /* '\' */
    {0x00,0x41,0x41,0x7F,0x00}, /* ']' */
    {0x04,0x02,0x01,0x02,0x04}, /* '^' */
    {0x40,0x40,0x40,0x40,0x40}, /* '_' */
    {0x00,0x01,0x02,0x04,0x00}, /* '`' */
    {0x20,0x54,0x54,0x54,0x78}, /* 'a' */
    {0x7F,0x48,0x44,0x44,0x38}, /* 'b' */
    {0x38,0x44,0x44,0x44,0x20}, /* 'c' */
    {0x38,0x44,0x44,0x48,0x7F}, /* 'd' */
    {0x38,0x54,0x54,0x54,0x18}, /* 'e' */
    {0x08,0x7E,0x09,0x01,0x02}, /* 'f' */
    {0x0C,0x52,0x52,0x52,0x3E}, /* 'g' */
    {0x7F,0x08,0x04,0x04,0x78}, /* 'h' */
    {0x00,0x44,0x7D,0x40,0x00}, /* 'i' */
    {0x20,0x40,0x44,0x3D,0x00}, /* 'j' */
    {0x7F,0x10,0x28,0x44,0x00}, /* 'k' */
    {0x00,0x41,0x7F,0x40,0x00}, /* 'l' */
    {0x7C,0x04,0x18,0x04,0x78}, /* 'm' */
    {0x7C,0x08,0x04,0x04,0x78}, /* 'n' */
    {0x38,0x44,0x44,0x44,0x38}, /* 'o' */
    {0x7C,0x14,0x14,0x14,0x08}, /* 'p' */
    {0x08,0x14,0x14,0x18,0x7C}, /* 'q' */
    {0x7C,0x08,0x04,0x04,0x08}, /* 'r' */
    {0x48,0x54,0x54,0x54,0x20}, /* 's' */
    {0x04,0x3F,0x44,0x40,0x20}, /* 't' */
    {0x3C,0x40,0x40,0x20,0x7C}, /* 'u' */
    {0x1C,0x20,0x40,0x20,0x1C}, /* 'v' */
    {0x3C,0x40,0x30,0x40,0x3C}, /* 'w' */
    {0x44,0x28,0x10,0x28,0x44}, /* 'x' */
    {0x0C,0x50,0x50,0x50,0x3C}, /* 'y' */
    {0x44,0x64,0x54,0x4C,0x44}, /* 'z' */

};

GPIOx_PinConfig_t TFT_RST_PIN={
		.Port = GPIO_PORTA,
		.Pin = GPIO_PIN2,
		.Mode = GPIO_MODE_OUTPUT,
		.Speed = GPIO_SPEED_HIGH,
		.OutputType = GPIO_OUTPUT_PUSHPULL
};

GPIOx_PinConfig_t TFT_A0_PIN={
		.Port = GPIO_PORTA,
		.Pin = GPIO_PIN1,
		.Mode = GPIO_MODE_OUTPUT,
		.Speed = GPIO_SPEED_HIGH,
		.OutputType = GPIO_OUTPUT_PUSHPULL
};

static void Reset_Seq(void)
{
	//RST PIN 1
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	MSYSTICK_vSetDelay_us(100);

	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_LOW);
	MSYSTICK_vSetDelay_us(1);

	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	MSYSTICK_vSetDelay_us(100);

	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_LOW);
	MSYSTICK_vSetDelay_us(100);

	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	MSYSTICK_vSetDelay_ms(120);
}

static void Write_cmd(u8 A_u8cmd)
{
	MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_LOW);
	MSPI_u8Transceive(A_u8cmd);
}

static void Write_data(u8 A_u8data)
{
	MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_HIGH);
	MSPI_u8Transceive(A_u8data);
}

void HTFT_vInit(void)
{
	MGPIO_vPinInit(&TFT_RST_PIN);
	MGPIO_vPinInit(&TFT_A0_PIN);

	MSPI_vInit();

	MSYSTICK_Config_t STK_CFG ={
			.InterruptEnable = INT_DISABLE,
			.CLK_SRC = CLK_SRC_AHB_8
	};
	 MSYSTICK_vInit(&STK_CFG);

	//RST
	Reset_Seq();

	//SLEEP OUT (CMD)
	Write_cmd(0x11);

	//WAIT 15 MSEC
	MSYSTICK_vSetDelay_ms(15);

	//SELECT MODE
	Write_cmd(0x3A);
	//MODE IS 565->0X05
	Write_data(0x05);

	//DISPLAY ON
	Write_cmd(0x29);
}

void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize)
{
	u8 MSB=0;
	u8 LSB=0;
	//Set X Position
	Write_cmd(0x2A);

	//Send X Start
	//MSB
	Write_data(0);
	//LSB
	Write_data(0);

	//Send X End
	//127 = 0x00 7F
	Write_data(0);
	Write_data(127);

	//Set Y Position
	Write_cmd(0x2B);

	//Send Y Start
	//MSB
	Write_data(0);
	//LSB
	Write_data(0); //start

	//Send Y End
	//159 = 0x00 9F
	Write_data(0);
	Write_data(159); //end

	//Send Image
	Write_cmd(0x2C);

	for(u16 i=0; i<A_u16ImgSize; i++)
	{
		MSB = (A_u16ImgArr[i] & 0xFF00) >>8;
		LSB = A_u16ImgArr[i] & 0x00FF;

		Write_data(MSB);
		Write_data(LSB);
	}
}

void HTFT_vSetXPos(u16 A_u16xStart, u16 A_u16xEnd)
{
	CurrentXStart = A_u16xStart;
	CurrentXEnd = A_u16xEnd;

	Write_cmd(0x2A);
	Write_data((u8)(A_u16xStart >> 8));
	Write_data((u8)A_u16xStart);
	Write_data((u8)(A_u16xEnd >> 8));
	Write_data((u8)A_u16xEnd);

}
void HTFT_vSetYPos(u16 A_u16yStart, u16 A_u16yEnd)
{
	CurrentYStart = A_u16yStart;
	CurrentYEnd = A_u16yEnd;

	Write_cmd(0x2B);
	Write_data((u8)(A_u16yStart >> 8));
	Write_data((u8)A_u16yStart);
	Write_data((u8)(A_u16yEnd >> 8));
	Write_data((u8)A_u16yEnd);
}
void HTFT_vFillBackgroundColor(u16 A_u16Color)
{
	HTFT_vSetXPos(0, 127);
	HTFT_vSetYPos(0, 159);

	HTFT_vFillRectangle(A_u16Color);
}
void HTFT_vFillRectangle(u16 A_u16Color)
{
	u8 MSB= 0;
	u8 LSB=0;
	u16 Pixels = (CurrentXEnd - CurrentXStart+1) * (CurrentYEnd - CurrentYStart+1);

	Write_cmd(0x2C);
	for(u16 i=0; i<Pixels; i++)
	{
		MSB = (A_u16Color & 0xFF00) >>8;
		LSB = A_u16Color & 0x00FF;

		Write_data(MSB);
		Write_data(LSB);
	}
}

void HTFT_vDrawRect(u16 A_u16X, u16 A_u16Y, u16 A_u16Width, u16 A_u16Height, u16 A_u16Color)
{
    // Draw Top and Bottom edges
    for (u16 i = A_u16X; i < A_u16X + A_u16Width; i++)
    {
        HTFT_vDrawPixel(i, A_u16Y, A_u16Color);
        HTFT_vDrawPixel(i, A_u16Y + A_u16Height - 1, A_u16Color);
    }

    // Draw Left and Right edges
    for (u16 i = A_u16Y; i < A_u16Y + A_u16Height; i++)
    {
        HTFT_vDrawPixel(A_u16X, i, A_u16Color);
        HTFT_vDrawPixel(A_u16X + A_u16Width - 1, i, A_u16Color);
    }
}

void HTFT_vDrawSolidRect(u16 A_u16X, u16 A_u16Y, u16 A_u16Width, u16 A_u16Height, u16 A_u16Color)
{
    // Configure the TFT window to exactly the size of our desired rectangle
    HTFT_vSetXPos(A_u16X, A_u16X + A_u16Width - 1);
    HTFT_vSetYPos(A_u16Y, A_u16Y + A_u16Height - 1);

    // Use your existing hardware fill command to flood the window
    HTFT_vFillRectangle(A_u16Color);
}

void HTFT_vDrawPixel(u16 A_u16X, u16 A_u16Y, u16 A_u16Color)
{
    // Set the specific pixel as both the start and end of the drawing window
    HTFT_vSetXPos(A_u16X, A_u16X);
    HTFT_vSetYPos(A_u16Y, A_u16Y);

    // Command to write to RAM
    Write_cmd(0x2C);

    // Send 16-bit color data (MSB first, then LSB)
    Write_data((u8)(A_u16Color >> 8));
    Write_data((u8)(A_u16Color & 0x00FF));
}

void HTFT_vPrintChar(u8 A_u8Char, u16 A_u16X, u16 A_u16Y, u16 A_u16Color, u16 A_u16BgColor)
{
    // Ensure character is within standard ASCII range
    if (A_u8Char < ' ' || A_u8Char > '~') return;

    u8 Local_u8CharIndex = A_u8Char - ' ';

    // 1. Set a drawing window exactly the size of the 5x8 character
    HTFT_vSetXPos(A_u16X, A_u16X + 4);
    HTFT_vSetYPos(A_u16Y, A_u16Y + 7);

    // 2. Send the Memory Write command
    Write_cmd(0x2C);

    // 3. Stream the pixel colors (Outer loop: Rows/Y, Inner loop: Cols/X)
    for (u8 Local_u8Row = 0; Local_u8Row < 8; Local_u8Row++)
    {
        for (u8 Local_u8Col = 0; Local_u8Col < 5; Local_u8Col++)
        {
            // Extract the specific bit for this pixel from the font array
            u8 Local_u8PixelData = Font5x8[Local_u8CharIndex][Local_u8Col];

            if ((Local_u8PixelData >> Local_u8Row) & 0x01)
            {
                // Draw Foreground Color
                Write_data((u8)(A_u16Color >> 8));
                Write_data((u8)(A_u16Color & 0x00FF));
            }
            else
            {
                // Draw Background Color
                Write_data((u8)(A_u16BgColor >> 8));
                Write_data((u8)(A_u16BgColor & 0x00FF));
            }
        }
    }
}

void HTFT_vPrintString(const char* A_pu8String, u16 A_u16X, u16 A_u16Y, u16 A_u16Color, u16 A_u16BgColor)
{
    u16 Local_u16CurrentX = A_u16X;
    u16 Local_u16CurrentY = A_u16Y;

    while (*A_pu8String != '\0')
    {
        HTFT_vPrintChar(*A_pu8String, Local_u16CurrentX, Local_u16CurrentY, A_u16Color, A_u16BgColor);

        // Advance cursor by 6 pixels (5 for the character + 1 for spacing)
        Local_u16CurrentX += 6;

        // Basic wrapping text logic to prevent drawing off the 128px screen edge
        if (Local_u16CurrentX >= 122)
        {
            Local_u16CurrentX = 0;
            Local_u16CurrentY += 10; // 8 pixels height + 2 pixels spacing
        }

        A_pu8String++;
    }
}

void HTFT_vPrintNumber(f32 A_f32Number, u16 A_u16X, u16 A_u16Y, u16 A_u16Color, u16 A_u16BgColor)
{
    u8 Local_chBuffer[20];

    // 1. Extract the integer part
    u32 IntPart = (u32)A_f32Number;

    // 2. Extract the fractional part (multiply by 100 for 2 decimal places)
    f32 Remainder = A_f32Number - (f32)IntPart;
    u32 FracPart = (u32)(Remainder * 100);

    // Ensure the fractional part is positive if the number was negative
    if (FracPart < 0)
    {
        FracPart = -FracPart;
    }

    // 3. Print them together using standard integer formatting
    // %02d ensures it prints "3.05" instead of "3.5"
    sprintf(Local_chBuffer, "%ld.%02ld", IntPart, FracPart);

    HTFT_vPrintString(Local_chBuffer, A_u16X, A_u16Y, A_u16Color, A_u16BgColor);
}

void HTFT_vDrawLine(u16 A_u16X0, u16 A_u16Y0, u16 A_u16X1, u16 A_u16Y1, u16 A_u16Color)
{
    // Calculate absolute differences without relying on <stdlib.h>
    int dx = (A_u16X1 > A_u16X0) ? (A_u16X1 - A_u16X0) : (A_u16X0 - A_u16X1);
    int sx = A_u16X0 < A_u16X1 ? 1 : -1;

    // Note: dy is intentionally negative for Bresenham's algorithm
    int dy = (A_u16Y1 > A_u16Y0) ? -(A_u16Y1 - A_u16Y0) : -(A_u16Y0 - A_u16Y1);
    int sy = A_u16Y0 < A_u16Y1 ? 1 : -1;

    int err = dx + dy, e2;

    while (1)
    {
        HTFT_vDrawPixel(A_u16X0, A_u16Y0, A_u16Color);

        if (A_u16X0 == A_u16X1 && A_u16Y0 == A_u16Y1) break;

        e2 = 2 * err;
        if (e2 >= dy) { err += dy; A_u16X0 += sx; }
        if (e2 <= dx) { err += dx; A_u16Y0 += sy; }
    }
}

// ============= INTERFACE FUNCTIONS =============

//void TFT_vDashBoard(void) //1
//{
//	//DISPLAY "DASH BOARD"
//	HTFT_vPrintString("DASH BOARD", 35, 20, TFT_BLUE, TFT_BLACK);
//
//    // DISPLAY "HEART RATE"
//	HTFT_vPrintString("HEART RATE:", 5, 45, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(80.56, 71, 45, TFT_GREEN, TFT_BLACK);
//	HTFT_vPrintString("BPM", 105, 45, TFT_WHITE, TFT_BLACK);
//
//	//SPO2
//	HTFT_vPrintString("SPO2:", 5, 70, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(60, 30, 70, TFT_GREEN, TFT_BLACK);
//
//	//STEPS
//	HTFT_vPrintString("STEPS:", 5, 95, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(110, 43, 70, TFT_GREEN, TFT_BLACK);
//	HTFT_vPrintString("STEP", 82, 95, TFT_WHITE, TFT_BLACK);
//
//	// DISTANCE
//	HTFT_vPrintString("DISTANCE:", 5, 120, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(50, 65, 120, TFT_GREEN, TFT_BLACK); //FROM SENSOR "m"
//	HTFT_vPrintString("m", 100, 120, TFT_WHITE, TFT_BLACK);
//
//	//CALORIES
//	HTFT_vPrintString("CALORIES:", 5, 145, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(99, 60, 145, TFT_GREEN, TFT_BLACK);
//	HTFT_vPrintString("kcal", 100, 145, TFT_WHITE, TFT_BLACK);
//}

//void TFT_vStepsReview(void) //2
//{
//	HTFT_vPrintString("STEPS COUNTER",30, 20, TFT_BLUE, TFT_BLACK);
//
//	HTFT_vPrintString("STEPS:", 5, 45, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(110, 50, 45, TFT_GREEN, TFT_BLACK); //FROM SENSOR
//
//	HTFT_vPrintString("DISTANCE:", 5, 70, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintNumber(50, 65, 70, TFT_GREEN, TFT_BLACK); //FROM SENSOR "m"
//	HTFT_vPrintString("m", 100, 70, TFT_WHITE, TFT_BLACK);
//
//
//	HTFT_vPrintString("ACTIVITY:", 5, 95, TFT_WHITE, TFT_BLACK); //WALKING OR RUNNING
//	HTFT_vPrintString("RUNNING", 60, 95, TFT_GREEN, TFT_BLACK);
//
//	HTFT_vPrintString("CALORIES:", 5, 120, TFT_WHITE, TFT_BLACK); //"kcal"
//	HTFT_vPrintNumber(99, 60, 120, TFT_GREEN, TFT_BLACK);
//	HTFT_vPrintString("kcal", 100, 120, TFT_WHITE, TFT_BLACK);
//}
//
//void TFT_vSettings(void) //3
//{
//	//TITLE
//	HTFT_vPrintString("SETTINGS", 40, 20, TFT_BLUE, TFT_BLACK);
//
//    // DISPLAY "HEART RATE"
//	HTFT_vPrintString("HR ALERT:", 5, 45, TFT_WHITE, TFT_BLACK);
//    // HEART RATE VALUE ->SENSOR
//	HTFT_vPrintNumber(120, 60, 45, TFT_GREEN, TFT_BLACK);
//    // Printing 'BPM' right next to the number
//	HTFT_vPrintString("BPM", 102, 45, TFT_WHITE, TFT_BLACK);
//
//	//BRIGHTNESS
//	HTFT_vPrintString("BRIGHTNESS:", 5, 70, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintString("80%", 75, 70, TFT_GREEN, TFT_BLACK);
//
//	//WIFI CONNECTION
//	HTFT_vPrintString("CONNECTION:", 5, 95, TFT_WHITE, TFT_BLACK);
//	HTFT_vPrintString("WIFI ON", 75, 95, TFT_GREEN, TFT_BLACK);
//}

void TFT_vGraph(void) //4
{
	HTFT_vPrintString("HISTORY", 40, 20, TFT_BLUE, TFT_BLACK);
	HTFT_vPrintString("HEART RATE", 35, 30, TFT_BLUE, TFT_BLACK);
    // Draw a hollow rectangle for the graph area
    // X=10, Y=40, Width=108, Height=80
    HTFT_vDrawRect(10, 50, 108, 80, TFT_WHITE);

    // Draw a faint center line for visual reference (e.g., 90 BPM baseline)
        HTFT_vDrawLine(10, 80, 118, 80, TFT_GREEN);

        // 4. Simulated Heart Rate Data (10 readings)
        u8 Dummy_BPM_Data[10] = { 72, 75, 80, 88, 95, 90, 85, 80, 78, 75 }; //GIVEN FROM THE SENSOR

        // 5. Plotting Logic
        u16 X_Start = 12; // Start just inside the left border
        u16 X_Step = 10;  // Pixels between each reading on the X-axis

        // Calculate the first point
        // We scale it so 60 BPM is the bottom of the box (Y=118) and 140 BPM is the top (Y=40)
        // Formula: BottomY - (BPM - MinBPM)
        u16 Previous_X = X_Start;
        u16 Previous_Y = 118 - (Dummy_BPM_Data[0] - 60);

        for (u8 i = 1; i < 10; i++)
        {
            // Calculate the next coordinate
            u16 Current_X = Previous_X + X_Step;
            u16 Current_Y = 118 - (Dummy_BPM_Data[i] - 60);

            // Draw the connecting line segment
            HTFT_vDrawLine(Previous_X, Previous_Y, Current_X, Current_Y, TFT_RED);

            // Update previous coordinates for the next loop iteration
            Previous_X = Current_X;
            Previous_Y = Current_Y;
        }
}

