/*
 * Activity_Tracker.h
 *
 *  Created on: Sep 13, 2026
 *      Author: Tona
 */

#ifndef APP_ACTIVITY_TRACKER_H_
#define APP_ACTIVITY_TRACKER_H_

#include "../LIB/STD_TYPES.h"

typedef enum {
    ACTIVITY_INACTIVE = 0,
    ACTIVITY_WALKING,
    ACTIVITY_RUNNING
} ActivityState_t;

typedef struct {
    u16 step_count;
    u16 walk_steps;
    u16 run_steps;
    f32 distance_meters;
    f32 calories_burned;
    ActivityState_t current_activity;
} ActivityTracker_t;

// User Profile Defaults (adjust to user height/weight)
#define USER_WEIGHT_KG          70.0f
#define STRIDE_WALK_METERS      0.70f   // 70 cm
#define STRIDE_RUN_METERS       1.00f   // 100 cm

// Calorie per step estimates based on MET formulas for 70kg adult
#define CALORIES_PER_WALK_STEP  0.040f  // kcal
#define CALORIES_PER_RUN_STEP   0.065f  // kcal

#define STEP_THRESHOLD_WALK_HI   1.10f   // Raised: Ignores resting hand movements
#define STEP_THRESHOLD_RUN_HI    1.50f   // Raised: Requires real running impact force
#define STEP_THRESHOLD_LO        1.05f   // Reset threshold (return near 1.0g gravity)
#define SMOOTHING_ALPHA          0.15f   // Low-pass filter weight

#define INACTIVITY_TIMEOUT_SAMPLES  150  // ~5 seconds (at ~60Hz loop rate)



#endif /* APP_ACTIVITY_TRACKER_H_ */
