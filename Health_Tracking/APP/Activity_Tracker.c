#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"


#include "../HAL/MPU6050/MPU_int.h"
#include "../HAL/TFT/TFT_int.h"

#include "Activity_Tracker.h"

#include <math.h>

// --- Adjusted Thresholds for Better Stability ---
//#define STEP_THRESHOLD_WALK_HI   1.30f   // Raised: Ignores resting hand movements
//#define STEP_THRESHOLD_RUN_HI    1.80f   // Raised: Requires real running impact force
//#define STEP_THRESHOLD_LO        1.05f   // Reset threshold (return near 1.0g gravity)
//#define SMOOTHING_ALPHA          0.15f   // Low-pass filter weight
//
//#define INACTIVITY_TIMEOUT_SAMPLES  300  // ~5 seconds (at ~60Hz loop rate)

// ActivityTracker_t tracker = {0};
// u8 Step_State = 0;
// f32 Filtered_Mag = 1.0f;
// f32 Peak_Hold_Mag = 0.0f;
// u16 Inactivity_Counter = 0;
// f32 Xg, Yg, Zg, magnitude;




/*    MRCC_vInit();
    MI2C1_vInit();
    HMPU_vInit();

    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_APB2, RCC_SPI1);

    // Initialise SPI GPIO Pins
    GPIOx_PinConfig_t MOSI = {.Port = GPIO_PORTA, .Pin = GPIO_PIN7, .Mode = GPIO_MODE_AF, .AltFunc = GPIO_AF5};
    GPIOx_PinConfig_t SCK  = {.Port = GPIO_PORTA, .Pin = GPIO_PIN5, .Mode = GPIO_MODE_AF, .AltFunc = GPIO_AF5};
    MGPIO_vPinInit(&MOSI);
    MGPIO_vPinInit(&SCK);

    HTFT_vInit();
*/





//        // 1. Read Acceleration
//        HMPU_vGetAcceleration(&Xg, &Yg, &Zg);
//
//        // 2. Compute 3D Magnitude & Low-Pass Filter
//        magnitude = sqrtf((Xg * Xg) + (Yg * Yg) + (Zg * Zg));
//        Filtered_Mag = (SMOOTHING_ALPHA * magnitude) + ((1.0f - SMOOTHING_ALPHA) * Filtered_Mag);
//
//        // 3. Peak Detection State Machine
//        if (Step_State == 0)
//        {
//            // Detect footstrike impact crossing 1.30g
//            if (Filtered_Mag > STEP_THRESHOLD_WALK_HI)
//            {
//                Step_State = 1;
//                //first
//                Peak_Hold_Mag = Filtered_Mag; // Lock in initial peak value
//            }
//            else
//            {
//                // Count idle loop samples when no step is happening
//                Inactivity_Counter++;
//                if (Inactivity_Counter >= INACTIVITY_TIMEOUT_SAMPLES)
//                {
//                    if (tracker.current_activity != ACTIVITY_INACTIVE)
//                    {
//                        tracker.current_activity = ACTIVITY_INACTIVE;
//                        HTFT_vPrintString("IDLE   ", 80, 70, 0x7BE0, 0x0000); // Display in Gray
//                    }
//                    Inactivity_Counter = INACTIVITY_TIMEOUT_SAMPLES; // Prevent overflow
//                }
//            }
//        }
//        else if (Step_State == 1)
//        {
//            // Track maximum impact force ONLY while step peak is active
//            if (Filtered_Mag > Peak_Hold_Mag)
//            {
//            	//second
//                Peak_Hold_Mag = Filtered_Mag;
//            }
//
//            // Detect step recovery (force drops back below 1.05g)
//            if (Filtered_Mag < STEP_THRESHOLD_LO)
//            {
//                tracker.step_count++;
//                Inactivity_Counter = 0; // Reset inactivity counter on confirmed step
//
//                // Classify activity based on peak force achieved during this step
//                if (Peak_Hold_Mag >= STEP_THRESHOLD_RUN_HI)
//                {
//                    tracker.current_activity = ACTIVITY_RUNNING;
//                    tracker.run_steps++;
//                    tracker.distance_meters += STRIDE_RUN_METERS;
//                    tracker.calories_burned  += CALORIES_PER_RUN_STEP;
//
//                    HTFT_vPrintString("RUNNING ", 80, 70, 0xF800, 0x0000); // Red
//                }
//
//                else
//                {
//                    tracker.current_activity = ACTIVITY_WALKING;
//                    tracker.walk_steps++;
//                    tracker.distance_meters += STRIDE_WALK_METERS;
//                    tracker.calories_burned  += CALORIES_PER_WALK_STEP;
//
//                    HTFT_vPrintString("WALKING ", 80, 70, 0x07E0, 0x0000); // Green
//                }
//
//                // Update numerical values on TFT display
//                HTFT_vPrintNumber((u16)tracker.step_count, 80, 10, 0x07E0, 0x0000);
//                HTFT_vPrintNumber((tracker.distance_meters / 1000), 80, 30, 0x07E0, 0x0000);
//                HTFT_vPrintNumber(tracker.calories_burned, 80, 50, 0x07E0, 0x0000);
//
//                // Reset state machine for next step
//                Step_State = 0;
//                Peak_Hold_Mag = 0.0f;
//            }
//        }



//
//void APP_vActivityTracker()
//{
//        // 1. Read Acceleration
//        HMPU_vGetAcceleration(&Xg, &Yg, &Zg);
//
//        // 2. Compute 3D Magnitude & Low-Pass Filter
//        magnitude = sqrtf((Xg * Xg) + (Yg * Yg) + (Zg * Zg));
//        Filtered_Mag = (SMOOTHING_ALPHA * magnitude) + ((1.0f - SMOOTHING_ALPHA) * Filtered_Mag);
//
//        // 3. Peak Detection State Machine
//        if (Step_State == 0)
//        {
//            // Detect footstrike impact crossing 1.30g
//            if (Filtered_Mag > STEP_THRESHOLD_WALK_HI)
//            {
//                Step_State = 1;
//                //first
//                Peak_Hold_Mag = Filtered_Mag; // Lock in initial peak value
//            }
//            else
//            {
//                // Count idle loop samples when no step is happening
//                Inactivity_Counter++;
//                if (Inactivity_Counter >= INACTIVITY_TIMEOUT_SAMPLES)
//                {
//                    if (tracker.current_activity != ACTIVITY_INACTIVE)
//                    {
//                        tracker.current_activity = ACTIVITY_INACTIVE;
//                        //HTFT_vPrintString("IDLE   ", 80, 70, 0x7BE0, 0x0000); // Display in Gray
//                    }
//                    Inactivity_Counter = INACTIVITY_TIMEOUT_SAMPLES; // Prevent overflow
//                }
//            }
//        }
//        else if (Step_State == 1)
//        {
//            // Track maximum impact force ONLY while step peak is active
//            if (Filtered_Mag > Peak_Hold_Mag)
//            {
//            	//second
//                Peak_Hold_Mag = Filtered_Mag;
//            }
//
//            // Detect step recovery (force drops back below 1.05g)
//            if (Filtered_Mag < STEP_THRESHOLD_LO)
//            {
//                tracker.step_count++;
//                Inactivity_Counter = 0; // Reset inactivity counter on confirmed step
//
//                // Classify activity based on peak force achieved during this step
//                if (Peak_Hold_Mag >= STEP_THRESHOLD_RUN_HI)
//                {
//                    tracker.current_activity = ACTIVITY_RUNNING;
//                    tracker.run_steps++;
//                    tracker.distance_meters += STRIDE_RUN_METERS;
//                    tracker.calories_burned  += CALORIES_PER_RUN_STEP;
//
//                   // HTFT_vPrintString("RUNNING ", 80, 70, 0xF800, 0x0000); // Red
//                }
//
//                else
//                {
//                    tracker.current_activity = ACTIVITY_WALKING;
//                    tracker.walk_steps++;
//                    tracker.distance_meters += STRIDE_WALK_METERS;
//                    tracker.calories_burned  += CALORIES_PER_WALK_STEP;
//
//                    //HTFT_vPrintString("WALKING ", 80, 70, 0x07E0, 0x0000); // Green
//                }
//
//                // Update numerical values on TFT display
//
//                TFT_vStepsReview(tracker.step_count, (tracker.distance_meters), tracker.calories_burned, tracker.current_activity);
//                /*HTFT_vPrintNumber((u16)tracker.step_count, 80, 10, 0x07E0, 0x0000);
//                HTFT_vPrintNumber((tracker.distance_meters / 1000), 80, 30, 0x07E0, 0x0000);
//                HTFT_vPrintNumber(tracker.calories_burned, 80, 50, 0x07E0, 0x0000);*/
//
//                // Reset state machine for next step
//                Step_State = 0;
//                Peak_Hold_Mag = 0.0f;
//            }
//        }
//    }
