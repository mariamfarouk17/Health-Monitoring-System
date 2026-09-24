/*
 * main.c
 *
 *  Created on: Sep 14, 2026
 *      Author: HP
 */
/*========= LIBRARIES =========*/
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include <math.h>

/*========= MCAL LAYER =========*/
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/I2C/I2C_int.h"
#include "../MCAL/SPI/SPI_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"

#include "../MCAL/SYSCFG/SYSCFG_int.h"

/*========= HAL LAYER =========*/
#include "../HAL/MPU6050/MPU_int.h"
#include "../HAL/TFT/TFT_int.h"

/*========= APP LAYER =========*/
#include "Activity_Tracker.h"

void APP_GET_TIME(void);
void APP_DecodeBits(void);
void APP_vActivityTracker();

volatile u8 G_u8StartingFlag =0;
volatile u32 G_u32Arr[50] = {0};
volatile u8 G_u8Counter =0;
volatile u8 G_u8NewCmdFlag =0;
volatile u8 G_u8Value =0;


ActivityTracker_t tracker = {0};
u8 Step_State = 0;
f32 Filtered_Mag = 1.0f;
f32 Peak_Hold_Mag = 0.0f;
u16 Inactivity_Counter = 0;
f32 Xg, Yg, Zg, magnitude;



#define DELAY_MS(d)   do{unsigned int i =d*4000; while(i--){asm("NOP");}}while(0)

int main()
{
	// Initialize RCC
    MRCC_vInit();

    // Enable Peripheral Clock
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTB);
    MRCC_vEnableCLK(RCC_APB2, 14);
    MRCC_vEnableCLK(RCC_APB2, 12);

    // GPIO Configuration for SPI
        GPIOx_PinConfig_t MOSI =
        {
    		.Port = GPIO_PORTA,
    		.Pin = GPIO_PIN7,
    		.Mode = GPIO_MODE_AF,
    		.AltFunc = GPIO_AF5
        };

        GPIOx_PinConfig_t SCK  =
        {
    		.Port = GPIO_PORTA,
    		.Pin = GPIO_PIN5,
    		.Mode = GPIO_MODE_AF,
    		.AltFunc = GPIO_AF5
        };

        MGPIO_vPinInit(&MOSI);
        MGPIO_vPinInit(&SCK);
        MSPI_vInit();

		// Initialize Peripherals
		MI2C1_vInit();
		HMPU_vInit();
		HTFT_vInit();

		HTFT_vFillBackgroundColor(TFT_BLACK);

		MSYSTICK_Config_t STK_cfg = {
			   .InterruptEnable = INT_ENABLE,
			   .CLK_SRC     = CLK_SRC_AHB_8
		};
	   MSYSTICK_vInit(&STK_cfg);

	   HTFT_vPrintString("DASH BOARD", 35, 20, TFT_BLUE, TFT_BLACK);

	   HTFT_vPrintString("HEART RATE:", 5, 40, TFT_WHITE, TFT_BLACK);
	   HTFT_vPrintString("SP02:", 5, 60, TFT_WHITE, TFT_BLACK);

		HTFT_vPrintString("STEPS:", 5, 80, TFT_WHITE, TFT_BLACK);
		HTFT_vPrintString("DISTANCE:", 5, 100, TFT_WHITE, TFT_BLACK);
		HTFT_vPrintString("CALORIES:", 5, 120, TFT_WHITE, TFT_BLACK);
		HTFT_vPrintString("ACTIVITY:", 5, 140, TFT_WHITE, TFT_BLACK);


	while(1)
	{
		APP_vActivityTracker();

		//STEPS
		HTFT_vPrintNumber((u16)tracker.step_count, 43, 80, TFT_GREEN, TFT_BLACK);
		HTFT_vPrintString("STEP", 82, 80, TFT_WHITE, TFT_BLACK);

		// DISTANCE
		HTFT_vPrintNumber((tracker.distance_meters / 1000), 65, 100, TFT_GREEN, TFT_BLACK); //FROM SENSOR "m"
		HTFT_vPrintString("Km", 100, 100, TFT_WHITE, TFT_BLACK);

		//CALORIES
		HTFT_vPrintNumber(tracker.calories_burned, 60, 120, TFT_GREEN, TFT_BLACK);
		HTFT_vPrintString("kcal", 100, 120, TFT_WHITE, TFT_BLACK);

		switch(tracker.current_activity)
		{
		case 0:
			HTFT_vPrintString("INACTIVE", 60, 140, TFT_RED, TFT_BLACK);
			break;
		case 1:
			HTFT_vPrintString("WALKING ", 60, 140, TFT_BLUE, TFT_BLACK);
			break;
		case 2:
			HTFT_vPrintString("RUNNING ", 60, 140, TFT_GREEN, TFT_BLACK);
			break;
		}

	}

	return 0;
}


void APP_GET_TIME(void)
{
	if(G_u8StartingFlag == 0)
	{
		G_u8StartingFlag =1;
		MSYSTICK_vSetIntervalSingle(15, APP_DecodeBits);
	}
	else
	{
		G_u32Arr[G_u8Counter++] = MSYSTICK_u32GetElapsedTime_SingleShot()/3.125;
		MSYSTICK_vSetIntervalSingle(4, APP_DecodeBits);
	}
}

// ================= SysTick Callback: Frame Decoder =================

void APP_DecodeBits(void)
{
	for(u8 i=0; i<8; i++)
	{
		//   1
		//   0             1                     001010
		//   1120          2270
		if(G_u32Arr[17+i]>=1000 && G_u32Arr[17+i]<=1250)
		{
			CLR_BIT(G_u8Value, i);  //Logical '0'
		}
		else if(G_u32Arr[17+i]>=2000 && G_u32Arr[17+i]<=2450)
		{
			SET_BIT(G_u8Value, i);   //Logical '1'
		}

	}
	G_u8StartingFlag =0;
	G_u8Counter=0;
	G_u8NewCmdFlag=1;

	for(u8 i=0; i<50;i++)
	{
		G_u32Arr[i]=0;
	}
}



void APP_vActivityTracker()
{
	// 1. Read Acceleration
	HMPU_vGetAcceleration(&Xg, &Yg, &Zg);

	// 2. Compute 3D Magnitude & Low-Pass Filter
	magnitude = sqrtf((Xg * Xg) + (Yg * Yg) + (Zg * Zg));
	Filtered_Mag = (SMOOTHING_ALPHA * magnitude) + ((1.0f - SMOOTHING_ALPHA) * Filtered_Mag);

	// 3. Peak Detection State Machine
	if (Step_State == 0)
	{
		// Detect footstrike impact crossing 1.30g
		if (Filtered_Mag > STEP_THRESHOLD_WALK_HI)
		{
			Step_State = 1;
			//first
			Peak_Hold_Mag = Filtered_Mag; // Lock in initial peak value
		}
		else
		{
			// Count idle loop samples when no step is happening
			Inactivity_Counter++;
			if (Inactivity_Counter >= INACTIVITY_TIMEOUT_SAMPLES)
			{
				if (tracker.current_activity != ACTIVITY_INACTIVE)
				{
					tracker.current_activity = ACTIVITY_INACTIVE;
					//HTFT_vPrintString("IDLE   ", 80, 70, 0x7BE0, 0x0000); // Display in Gray
				}
				Inactivity_Counter = INACTIVITY_TIMEOUT_SAMPLES; // Prevent overflow
			}
		}
	}
	else if (Step_State == 1)
	{
		// Track maximum impact force ONLY while step peak is active
		if (Filtered_Mag > Peak_Hold_Mag)
		{
			//second
			Peak_Hold_Mag = Filtered_Mag;
		}

		// Detect step recovery (force drops back below 1.05g)
		if (Filtered_Mag < STEP_THRESHOLD_LO)
		{
			tracker.step_count++;
			Inactivity_Counter = 0; // Reset inactivity counter on confirmed step

			// Classify activity based on peak force achieved during this step
			if (Peak_Hold_Mag >= STEP_THRESHOLD_RUN_HI)
			{
				tracker.current_activity = ACTIVITY_RUNNING;
				tracker.run_steps++;
				tracker.distance_meters += STRIDE_RUN_METERS;
				tracker.calories_burned  += CALORIES_PER_RUN_STEP;

			   // HTFT_vPrintString("RUNNING ", 80, 70, 0xF800, 0x0000); // Red
			}

			else
			{
				tracker.current_activity = ACTIVITY_WALKING;
				tracker.walk_steps++;
				tracker.distance_meters += STRIDE_WALK_METERS;
				tracker.calories_burned  += CALORIES_PER_WALK_STEP;
			}

			// Reset state machine for next step
			Step_State = 0;
			Peak_Hold_Mag = 0.0f;
		}
	}
}




///*
// * main.c
// *
// * MAX30102 + VITALS
// */
//
//
//#define F_CPU 16000000UL
//
//#include "../LIB/STD_TYPES.h"
//
//#include "../MCAL/RCC/RCC_int.h"
//#include "../MCAL/GPIO/GPIO_int.h"
//#include "../MCAL/STK/STK_int.h"
//#include "../MCAL/USART/USART_int.h"
//#include "../MCAL/I2C_MAX/I2C_MAX_int.h"
//
//#include "../HAL/MAX30102/MAX30102_int.h"
//
//#include "../APP/VITALS/VITALS_int.h"
//
//
///* =========================================================
// * MAX30102
// * ========================================================= */
//
//#define MAX30102_I2C_ADDR          0x57U
//
//#define MAX30102_REG_INT_STATUS1   0x00U
//#define MAX30102_REG_INT_ENABLE1   0x02U
//
//#define MAX30102_INT_PPG_RDY       0x40U
//
//
///* =========================================================
// * Send Number
// * ========================================================= */
//
//void SendNumber(
//    u32 A_u32Number
//)
//{
//    char L_u8Buffer[11];
//
//    u8 L_u8Index = 0U;
//
//
//    if(A_u32Number == 0U)
//    {
//        MUSART_vSendData('0');
//
//        return;
//    }
//
//
//    while(A_u32Number > 0U)
//    {
//        L_u8Buffer[L_u8Index] =
//            (char)(
//                '0' +
//                (A_u32Number % 10U)
//            );
//
//        A_u32Number /= 10U;
//
//        L_u8Index++;
//    }
//
//
//    while(L_u8Index > 0U)
//    {
//        L_u8Index--;
//
//        MUSART_vSendData(
//            L_u8Buffer[L_u8Index]
//        );
//    }
//}
//
//
///* =========================================================
// * Main
// * ========================================================= */
//
//int main(void)
//{
//    u8 L_u8Status;
//    u8 L_u8IntStatus1;
//    u8 L_u8PA0;
//
//    u8 L_u8Measuring = 0U;
//
//    u16 L_u16HR = 0U;
//
//    u8 L_u8SpO2 = 0U;
//    u8 L_u8SpO2Status;
//
//    MAX30102_Sample_t L_xSample;
//
//
//    /* =====================================================
//     * RCC
//     * ===================================================== */
//
//    MRCC_vInit();
//
//
//    MRCC_vEnableCLK(
//        RCC_AHB1,
//        RCC_GPIOA
//    );
//
//
//    /* =====================================================
//     * ADDED:
//     * Enable GPIOB clock for HR LEDs
//     * ===================================================== */
//
//    MRCC_vEnableCLK(
//        RCC_AHB1,
//        RCC_GPIOB
//    );
//
//
//    MRCC_vEnableCLK(
//        RCC_APB2,
//        RCC_USART1_BIT
//    );
//
//
//    /* =====================================================
//     * SysTick
//     * ===================================================== */
//
//    MSTK_Config_t L_xSTKConfig =
//    {
//        .InterruptEnable = INT_DISABLE,
//        .CLK_SRC = CLK_SRC_AHB_8
//    };
//
//
//    MSTK_vInit(
//        &L_xSTKConfig
//    );
//
//
//    /* =====================================================
//     * USART1 TX -> PA9
//     * ===================================================== */
//
//    GPIOx_PinConfig_t L_xUSART1Tx =
//    {
//        .Port       = GPIO_PORTA,
//        .Pin        = GPIO_PIN9,
//        .Mode       = GPIO_MODE_AF,
//        .OutputType = GPIO_OUTPUT_PUSHPULL,
//        .PullType   = GPIO_PULL_UP,
//        .Speed      = GPIO_SPEED_HIGH,
//        .AltFunc    = GPIO_AF7
//    };
//
//
//    MGPIO_vPinInit(
//        &L_xUSART1Tx
//    );
//
//
//    MUSART_vInit();
//
//
//    /* =====================================================
//     * BUTTON -> PA3
//     * ===================================================== */
//
//    GPIOx_PinConfig_t L_xButton =
//    {
//        .Port       = GPIO_PORTA,
//        .Pin        = GPIO_PIN3,
//        .Mode       = GPIO_MODE_INPUT,
//        .OutputType = GPIO_OUTPUT_PUSHPULL,
//        .PullType   = GPIO_PULL_UP,
//        .Speed      = GPIO_SPEED_LOW,
//        .AltFunc    = GPIO_AF0
//    };
//
//
//    MGPIO_vPinInit(
//        &L_xButton
//    );
//
//
//    /* =====================================================
//     * ADDED:
//     * HIGH HEART RATE LED -> PB12
//     * LOW HEART RATE LED  -> PB13
//     * ===================================================== */
//
//    GPIOx_PinConfig_t LED_HIGH =
//    {
//        .Port       = GPIO_PORTB,
//        .Pin        = GPIO_PIN12,
//        .Mode       = GPIO_MODE_OUTPUT,
//        .Speed      = GPIO_SPEED_LOW,
//        .OutputType = GPIO_OUTPUT_PUSHPULL,
//        .PullType   = GPIO_NO_PULL,
//        .AltFunc    = GPIO_AF0
//    };
//
//
//    MGPIO_vPinInit(
//        &LED_HIGH
//    );
//
//
//    GPIOx_PinConfig_t LED_LOW =
//    {
//        .Port       = GPIO_PORTB,
//        .Pin        = GPIO_PIN13,
//        .Mode       = GPIO_MODE_OUTPUT,
//        .Speed      = GPIO_SPEED_LOW,
//        .OutputType = GPIO_OUTPUT_PUSHPULL,
//        .PullType   = GPIO_NO_PULL,
//        .AltFunc    = GPIO_AF0
//    };
//
//
//    MGPIO_vPinInit(
//        &LED_LOW
//    );
//
//
//    /* =====================================================
//     * ADDED:
//     * Initially turn both LEDs OFF
//     * ===================================================== */
//
//    MGPIO_vSetPinValue(
//        GPIO_PORTB,
//        GPIO_PIN10,
//        GPIO_LOW
//    );
//
//
//    MGPIO_vSetPinValue(
//        GPIO_PORTB,
//        GPIO_PIN11,
//        GPIO_LOW
//    );
//
//
//    /* =====================================================
//     * I2C1
//     * ===================================================== */
//
//    MI2C2_MAX_vInit();
//
//
//    /* =====================================================
//     * MAX30102 INIT
//     * ===================================================== */
//
//    MUSART_vSendString(
//        "MAX INIT...\r\n"
//    );
//
//
//    L_u8Status =
//        HMAX30102_u8Init();
//
//
//    if(L_u8Status != 0U)
//    {
//        MUSART_vSendString(
//            "MAX INIT ERROR\r\n"
//        );
//
//        while(1)
//        {
//        }
//    }
//
//
//    MUSART_vSendString(
//        "MAX INIT OK\r\n"
//    );
//
//
//    /* =====================================================
//     * VITALS INIT
//     * ===================================================== */
//
//    MVITALS_vInit();
//
//
//    MUSART_vSendString(
//        "PRESS BUTTON TO START\r\n"
//    );
//
//
//    /* =====================================================
//     * MAIN LOOP
//     * ===================================================== */
//
//    while(1)
//    {
//        /* =================================================
//         * BUTTON PRESSED
//         * ================================================= */
//
//        if(
//            MGPIO_u8GetPinValue(
//                GPIO_PORTA,
//                GPIO_PIN3
//            ) == GPIO_LOW
//        )
//        {
//            /* =============================================
//             * Button debounce
//             * ============================================= */
//
//            MSTK_vSetDelay_ms(
//                50U
//            );
//
//
//            /* =============================================
//             * Make sure button is still pressed
//             * ============================================= */
//
//            if(
//                MGPIO_u8GetPinValue(
//                    GPIO_PORTA,
//                    GPIO_PIN3
//                ) == GPIO_LOW
//            )
//            {
//                /* =========================================
//                 * START VITALS
//                 * ========================================= */
//
//                if(
//                    L_u8Measuring == 0U
//                )
//                {
//                    MUSART_vSendString(
//                        "BUTTON PRESSED\r\n"
//                    );
//
//
//                    MUSART_vSendString(
//                        "START VITALS\r\n"
//                    );
//
//
//                    /* =====================================
//                     * Clear old MAX30102 interrupt
//                     * ===================================== */
//
//                    L_u8IntStatus1 = 0U;
//
//
//                    L_u8Status =
//                        MI2C2_MAX_u8ReadReg(
//                            MAX30102_I2C_ADDR,
//                            MAX30102_REG_INT_STATUS1,
//                            &L_u8IntStatus1
//                        );
//
//
//                    if(
//                        L_u8Status != I2C_MAX_OK
//                    )
//                    {
//                        MUSART_vSendString(
//                            "STATUS CLEAR ERROR\r\n"
//                        );
//
//                        while(1)
//                        {
//                        }
//                    }
//
//
//                    /* =====================================
//                     * Enable PPG_RDY
//                     * ===================================== */
//
//                    L_u8Status =
//                        MI2C2_MAX_u8WriteReg(
//                            MAX30102_I2C_ADDR,
//                            MAX30102_REG_INT_ENABLE1,
//                            MAX30102_INT_PPG_RDY
//                        );
//
//
//                    MUSART_vSendString(
//                        "PPG ENABLE STATUS = "
//                    );
//
//
//                    SendNumber(
//                        L_u8Status
//                    );
//
//
//                    MUSART_vSendString(
//                        "\r\n"
//                    );
//
//
//                    if(
//                        L_u8Status != I2C_MAX_OK
//                    )
//                    {
//                        MUSART_vSendString(
//                            "PPG ENABLE ERROR\r\n"
//                        );
//
//                        while(1)
//                        {
//                        }
//                    }
//
//
//                    /* =====================================
//                     * Check PA0
//                     * ===================================== */
//
//                    L_u8PA0 =
//                        MGPIO_u8GetPinValue(
//                            GPIO_PORTA,
//                            GPIO_PIN0
//                        );
//
//
//                    MUSART_vSendString(
//                        "PA0 = "
//                    );
//
//
//                    SendNumber(
//                        L_u8PA0
//                    );
//
//
//                    MUSART_vSendString(
//                        "\r\n"
//                    );
//
//
//                    /* =====================================
//                     * Start measuring
//                     * ===================================== */
//
//                    L_u8Measuring = 1U;
//
//
//                    MUSART_vSendString(
//                        "VITALS CALCULATION\r\n"
//                    );
//                }
//
//
//                /* =========================================
//                 * STOP VITALS
//                 * ========================================= */
//
//                else
//                {
//                    MUSART_vSendString(
//                        "BUTTON PRESSED\r\n"
//                    );
//
//
//                    MUSART_vSendString(
//                        "STOP VITALS\r\n"
//                    );
//
//
//                    /* =====================================
//                     * Disable PPG_RDY interrupt
//                     * ===================================== */
//
//                    L_u8Status =
//                        MI2C2_MAX_u8WriteReg(
//                            MAX30102_I2C_ADDR,
//                            MAX30102_REG_INT_ENABLE1,
//                            0x00U
//                        );
//
//
//                    MUSART_vSendString(
//                        "PPG DISABLE STATUS = "
//                    );
//
//
//                    SendNumber(
//                        L_u8Status
//                    );
//
//
//                    MUSART_vSendString(
//                        "\r\n"
//                    );
//
//
//                    L_u8Measuring = 0U;
//
//
//                    /* =====================================
//                     * ADDED:
//                     * Turn both HR LEDs OFF when stopped
//                     * ===================================== */
//
//                    MGPIO_vSetPinValue(
//                        GPIO_PORTB,
//                        GPIO_PIN12,
//                        GPIO_LOW
//                    );
//
//
//                    MGPIO_vSetPinValue(
//                        GPIO_PORTB,
//                        GPIO_PIN13,
//                        GPIO_LOW
//                    );
//
//
//                    MUSART_vSendString(
//                        "VITALS STOPPED\r\n"
//                    );
//                }
//
//
//                /* =========================================
//                 * Wait for button release
//                 * ========================================= */
//
//                while(
//                    MGPIO_u8GetPinValue(
//                        GPIO_PORTA,
//                        GPIO_PIN3
//                    ) == GPIO_LOW
//                )
//                {
//                }
//
//
//                /* =========================================
//                 * Button debounce after release
//                 * ========================================= */
//
//                MSTK_vSetDelay_ms(
//                    50U
//                );
//
//
//                if(
//                    L_u8Measuring == 0U
//                )
//                {
//                    MUSART_vSendString(
//                        "PRESS BUTTON TO START\r\n"
//                    );
//                }
//            }
//        }
//
//
//        /* =================================================
//         * PROCESS MAX30102 WHILE MEASURING
//         * ================================================= */
//
//        if(
//            L_u8Measuring == 1U
//        )
//        {
//            if(
//                HMAX30102_u8ProcessInterrupt(
//                    &L_xSample
//                ) == I2C_MAX_OK
//            )
//            {
//                /* =========================================
//                 * Add sample to VITALS
//                 * ========================================= */
//
//                MVITALS_vAddSample(
//                    L_xSample.Red,
//                    L_xSample.Ir
//                );
//
//
//                /* =========================================
//                 * Calculate when buffer is full
//                 * ========================================= */
//
//                if(
//                    MVITALS_u8IsBufferFull()
//                )
//                {
//                    MUSART_vSendString(
//                        "\r\n"
//                    );
//
//                    MUSART_vSendString(
//                        "==============================\r\n"
//                    );
//
//                    MUSART_vSendString(
//                        "       VITALS RESULT\r\n"
//                    );
//
//                    MUSART_vSendString(
//                        "==============================\r\n"
//                    );
//
//
//                    /* =====================================
//                     * SpO2 FIRST
//                     *
//                     * HR resets the sample counter.
//                     * ===================================== */
//
//                    L_u8SpO2Status =
//                        MVITALS_u8CalculateSpO2(
//                            &L_u8SpO2
//                        );
//
//
//                    if(
//                        L_u8SpO2Status == 0U
//                    )
//                    {
//                        MUSART_vSendString(
//                            "SpO2 = "
//                        );
//
//
//                        SendNumber(
//                            L_u8SpO2
//                        );
//
//
//                        MUSART_vSendString(
//                            " %\r\n"
//                        );
//                    }
//
//                    else
//                    {
//                        MUSART_vSendString(
//                            "SpO2 CALCULATION ERROR = "
//                        );
//
//
//                        SendNumber(
//                            L_u8SpO2Status
//                        );
//
//
//                        MUSART_vSendString(
//                            "\r\n"
//                        );
//                    }
//
//
//                    /* =====================================
//                     * ORIGINAL HR CALCULATION
//                     * ===================================== */
//
//                    if(
//                        MVITALS_u8CalculateHR(
//                            &L_u16HR
//                        ) == 0U
//                    )
//                    {
//                        MUSART_vSendString(
//                            "HEART RATE = "
//                        );
//
//
//                        SendNumber(
//                            L_u16HR
//                        );
//
//
//                        MUSART_vSendString(
//                            " BPM\r\n"
//                        );
//
//
//                        /* =================================
//                         * ADDED:
//                         * HEART RATE LED CONTROL
//                         * ================================= */
//
//                        /* Turn both LEDs OFF first */
//
//                        MGPIO_vSetPinValue(
//                            GPIO_PORTB,
//                            GPIO_PIN12,
//                            GPIO_LOW
//                        );
//
//
//                        MGPIO_vSetPinValue(
//                            GPIO_PORTB,
//                            GPIO_PIN13,
//                            GPIO_LOW
//                        );
//
//
//                        /* ================================
//                         * HIGH HEART RATE
//                         * HR >= 120 BPM
//                         * ================================ */
//
//                        if(
//                            L_u16HR >= 120U
//                        )
//                        {
//                            MGPIO_vSetPinValue(
//                                GPIO_PORTB,
//                                GPIO_PIN12,
//                                GPIO_HIGH
//                            );
//                        }
//
//
//                        /* ================================
//                         * LOW HEART RATE
//                         * HR <= 60 BPM
//                         * ================================ */
//
//                        else if(
//                            L_u16HR <= 60U
//                        )
//                        {
//                            MGPIO_vSetPinValue(
//                                GPIO_PORTB,
//                                GPIO_PIN13,
//                                GPIO_HIGH
//                            );
//                        }
//
//
//                        /* ================================
//                         * NORMAL HEART RATE
//                         *
//                         * 61 - 119 BPM
//                         *
//                         * Both LEDs stay OFF.
//                         * ================================ */
//                    }
//
//                    else
//                    {
//                        MUSART_vSendString(
//                            "HR CALCULATION ERROR\r\n"
//                        );
//
//
//                        /* =================================
//                         * ADDED:
//                         * Turn LEDs OFF if HR calculation
//                         * fails.
//                         * ================================= */
//
//                        MGPIO_vSetPinValue(
//                            GPIO_PORTB,
//                            GPIO_PIN12,
//                            GPIO_LOW
//                        );
//
//
//                        MGPIO_vSetPinValue(
//                            GPIO_PORTB,
//                            GPIO_PIN13,
//                            GPIO_LOW
//                        );
//                    }
//
//
//                    MUSART_vSendString(
//                        "==============================\r\n"
//                    );
//
//
//                    MVITALS_vInit();
//                }
//            }
//        }
//    }
//}
