# STM32 Health Tracking & Activity Monitoring System

A bare-metal **ARM Cortex-M4 embedded health-tracking and activity-monitoring system** built around the **STM32F401CCU**, using a layered **MCAL / HAL / Application** architecture.

The system combines optical health monitoring using the **MAX30102** with motion-based activity tracking using the **MPU6050**. Health and activity information is processed by the STM32 and presented through a TFT dashboard, with additional heart-rate alerts and serial communication for debugging and monitoring.

The project was developed using low-level embedded C and custom peripheral drivers, providing direct control over the STM32 hardware without relying on high-level vendor or Arduino-style abstractions.

---

## Features

* ❤️ **Heart Rate Monitoring** — Uses the MAX30102 optical sensor to acquire PPG data and calculate heart rate.
* 🩸 **SpO₂ Monitoring** — Processes the MAX30102 red and infrared signals to calculate blood oxygen saturation.
* 🚶 **Step Counter** — Detects individual steps from MPU6050 acceleration data.
* 🏃 **Activity Classification** — Classifies detected movement as `INACTIVE`, `WALKING`, or `RUNNING`.
* 📏 **Distance Estimation** — Calculates traveled distance using activity-dependent stride lengths.
* 🔥 **Calorie Estimation** — Estimates calories burned using different values for walking and running steps.
* 📊 **TFT Dashboard** — Displays steps, distance, calories, and current activity in real time.
* 🚨 **Heart Rate Alerts** — Uses dedicated LEDs to indicate high and low heart-rate conditions.
* 🔘 **Button-Controlled Monitoring** — Starts and stops the MAX30102 health-monitoring process using a push button.
* 📡 **USART Monitoring** — Outputs sensor measurements, status information, and diagnostic messages through USART.
* ⚡ **PPG Ready Handling** — Uses the MAX30102 PPG-ready interrupt/status mechanism for sample acquisition.
* 🧠 **Signal Processing** — Uses magnitude calculation, low-pass filtering, and peak detection for motion analysis.
* ⚙️ **Bare-Metal Architecture** — Uses custom STM32 peripheral drivers and direct register-level configuration.
* 🧩 **Layered Driver Architecture** — Separates MCU peripherals, external hardware drivers, and application logic.

---

# System Architecture

The project follows a layered embedded software architecture:

```text
                         APPLICATION LAYER
                              main.c
                                │
              ┌─────────────────┴─────────────────┐
              │                                   │
              ▼                                   ▼
       Activity Tracker                       Health Monitoring
              │                                   │
              ▼                                   ▼
          MPU6050                              MAX30102
              │                                   │
              ▼                                   ▼
       Step Detection                           VITALS
       Activity State                       HR / SpO₂
       Distance / Calories                       │
              │                                   │
              └─────────────────┬─────────────────┘
                                │
                                ▼
                         TFT Dashboard
                                │
                                ▼
                           User Output


                        HARDWARE ABSTRACTION
        ┌────────────────────────────────────────────────┐
        │ MPU6050 │ MAX30102 │ TFT │ VITALS │ Other HAL │
        └────────────────────────────────────────────────┘
                                │
                                ▼

                    MICROCONTROLLER ABSTRACTION
        ┌────────────────────────────────────────────────┐
        │ RCC │ GPIO │ I2C │ SPI │ SysTick │ EXTI      │
        │ NVIC │ SYSCFG │ USART │ DMA │ Other MCAL     │
        └────────────────────────────────────────────────┘
                                │
                                ▼
                         STM32F401CCU
                         ARM Cortex-M4
```

Each layer has a specific responsibility.

For example:

* The **MAX30102 driver** handles communication with the sensor through the I²C interface.
* The **VITALS module** processes Red/IR samples to calculate HR and SpO₂.
* The **MPU6050 driver** provides acceleration measurements to the activity tracker.
* The **Activity Tracker** performs motion processing and step classification.
* The **TFT driver** provides the graphical user interface.
* The **application layer** integrates the different subsystems.

---

# Repository Structure

```text
├── LIB/
│   ├── STD_TYPES.h
│   └── BIT_MATH.h
│
├── MCAL/
│   ├── RCC/
│   │   ├── RCC_int.h
│   │   ├── RCC_prv.h
│   │   └── RCC_prg.c
│   │
│   ├── GPIO/
│   │   ├── GPIO_int.h
│   │   ├── GPIO_prv.h
│   │   └── GPIO_prg.c
│   │
│   ├── I2C/
│   │   ├── I2C_int.h
│   │   ├── I2C_prv.h
│   │   └── I2C_prg.c
│   │
│   ├── SPI/
│   ├── SYSTICK/
│   ├── EXTI/
│   ├── NVIC/
│   └── SYSCFG/
│
├── HAL/
│   ├── MAX30102/
│   │   ├── MAX30102_int.h
│   │   ├── MAX30102_prv.h
│   │   └── MAX30102_prg.c
│   │
│   ├── MPU6050/
│   │   ├── MPU_int.h
│   │   ├── MPU_prv.h
│   │   └── MPU_prg.c
│   │
│   └── TFT/
│       ├── TFT_int.h
│       ├── TFT_prv.h
│       └── TFT_prg.c
│
├── APP/
│   ├── VITALS/
│   │   ├── VITALS_int.h
│   │   └── VITALS_prg.c
│   │
│   └── Activity_Tracker/
│       ├── Activity_Tracker.h
│       └── Activity_Tracker.c
│
└── main.c
```

---

# Hardware Setup

## Main Controller

| Component    | Description   |
| ------------ | ------------- |
| MCU          | STM32F401CCU  |
| Architecture | ARM Cortex-M4 |
| Language     | Embedded C    |
| IDE          | STM32CubeIDE  |
| Programming  | Bare-Metal    |
| Debugging    | ST-LINK       |

---

# MAX30102 — Heart Rate & SpO₂ Sensor

The **MAX30102** is used as the primary optical health sensor.

It provides red and infrared photoplethysmography (PPG) data that is processed by the VITALS application module to calculate:

* Heart Rate
* SpO₂

### Connection

| MAX30102 Signal | STM32 Pin |
| --------------- | --------- |
| VIN             | 3.3V      |
| GND             | GND       |
| SCL             | PB6       |
| SDA             | PB7       |
| INT             | PA0       |

The MAX30102 I²C address is:

```text
0x57
```

The sensor communicates through a dedicated I²C interface.

---

# MAX30102 Driver

The MAX30102 HAL driver is responsible for the hardware-level interaction with the sensor.

Main operations include:

```text
HMAX30102_u8Init()
HMAX30102_u8ProcessInterrupt()
```

The driver handles:

1. MAX30102 initialization
2. Sensor register configuration
3. Interrupt/status handling
4. PPG sample acquisition
5. Red-channel data retrieval
6. IR-channel data retrieval

The resulting sample is represented using:

```text
MAX30102_Sample_t
```

with:

```text
Red
Ir
```

channels.

---

# VITALS Processing

The VITALS application module receives Red and IR samples from the MAX30102 driver.

The processing flow is:

```text
             MAX30102
                 │
                 ▼
          PPG Sample Ready
                 │
                 ▼
       ┌─────────────────────┐
       │   Red / IR Samples  │
       └──────────┬──────────┘
                  │
                  ▼
             VITALS Buffer
                  │
          ┌───────┴────────┐
          ▼                ▼
       SpO₂                 HR
          │                │
          └───────┬────────┘
                  ▼
            Health Results
```

The application collects samples until the processing buffer is full.

It then performs:

```text
SpO₂ Calculation
        │
        ▼
Heart Rate Calculation
```

The calculated values are sent through USART and can be used by the rest of the application.

---

# Heart Rate Alert System

The system provides visual heart-rate alerts using two LEDs.

| Condition    | LED           |
| ------------ | ------------- |
| HR ≥ 120 BPM | High-HR LED   |
| HR ≤ 60 BPM  | Low-HR LED    |
| 61–119 BPM   | Both LEDs OFF |

The current implementation uses:

```text
PB12 → High Heart Rate LED
PB13 → Low Heart Rate LED
```

The LEDs are turned off when monitoring is stopped or when heart-rate calculation fails.

These thresholds are **project/demo thresholds**, not medical diagnostic limits.

---

# User Control

A push button is used to start and stop the health-monitoring process.

```text
Button
  │
  ▼
 PA3
```

The button is configured with an internal pull-up and uses an active-low input.

### Button Operation

```text
              Button Press
                   │
                   ▼
          ┌─────────────────┐
          │ Measuring OFF ? │
          └────────┬────────┘
                   │ YES
                   ▼
          Start VITALS System
                   │
                   ▼
          Enable PPG Ready
                   │
                   ▼
          Process MAX30102
```

Pressing the button again stops the measurement process and disables the PPG-ready functionality.

---

# MPU6050 — Activity Tracking

The MPU6050 is used to measure 3-axis acceleration.

The application obtains:

```text
X acceleration
Y acceleration
Z acceleration
```

The three values are combined to calculate the acceleration magnitude:

```text
Magnitude = √(X² + Y² + Z²)
```

This magnitude is then passed through a low-pass filter to reduce rapid fluctuations.

---

# Motion Processing

The activity-tracking algorithm consists of several stages:

```text
MPU6050
   │
   ▼
X / Y / Z Acceleration
   │
   ▼
3D Magnitude
   │
   ▼
Low-Pass Filter
   │
   ▼
Peak Detection
   │
   ▼
Step Detection
   │
   ├──────────────┐
   ▼              ▼
 WALKING        RUNNING
   │              │
   └──────┬───────┘
          ▼
 Distance / Calories
```

---

# Step Detection Algorithm

The step detector uses a two-state machine.

```text
             ┌─────────────────────┐
             │    Step_State = 0   │
             │ Waiting for impact  │
             └──────────┬──────────┘
                        │
             Filtered magnitude
              > walking HI
                        │
                        ▼
             ┌─────────────────────┐
             │    Step_State = 1   │
             │   Peak Tracking     │
             └──────────┬──────────┘
                        │
             Filtered magnitude
              < walking LO
                        │
                        ▼
                  Step Confirmed
                        │
                        ▼
             Classify Walking/Running
                        │
                        ▼
                Step_State = 0
```

The algorithm tracks the maximum acceleration magnitude during the detected step.

This peak value is then used to classify the activity.

---

# Activity Classification

The current activity states are:

```text
ACTIVITY_INACTIVE = 0
ACTIVITY_WALKING  = 1
ACTIVITY_RUNNING  = 2
```

### Inactive

If no step is detected for the configured inactivity period:

```text
No step
  │
  ▼
Inactivity Counter
  │
  ▼
Timeout
  │
  ▼
INACTIVE
```

### Walking

A confirmed step whose peak magnitude does not reach the running threshold is classified as:

```text
WALKING
```

### Running

If the peak magnitude reaches the running threshold:

```text
Peak >= STEP_THRESHOLD_RUN_HI
             │
             ▼
          RUNNING
```

---

# Distance Calculation

Distance is accumulated according to the detected activity.

For walking:

```text
distance_meters += STRIDE_WALK_METERS
```

For running:

```text
distance_meters += STRIDE_RUN_METERS
```

Therefore, walking and running use different stride-length parameters.

The TFT dashboard converts the stored distance into kilometers for display.

---

# Calorie Calculation

Calories are also calculated according to the detected activity.

Walking:

```text
calories_burned += CALORIES_PER_WALK_STEP
```

Running:

```text
calories_burned += CALORIES_PER_RUN_STEP
```

This allows the system to maintain separate activity-dependent calorie estimates.

---

# TFT Dashboard

The TFT is the primary visual interface of the activity-tracking application.

At startup, the dashboard displays:

```text
+--------------------------------+
|          DASH BOARD            |
|                                |
| HEART RATE:                    |
| SP02:                          |
| STEPS:                         |
| DISTANCE:                      |
| CALORIES:                      |
| ACTIVITY:                      |
|                                |
+--------------------------------+
```

The active activity-tracking implementation continuously updates:

```text
STEPS
DISTANCE
CALORIES
ACTIVITY
```

Example:

```text
DASH BOARD

HEART RATE:
SP02:
STEPS:       125 STEP
DISTANCE:    0 Km
CALORIES:    5 kcal
ACTIVITY:    WALKING
```

The activity state is displayed using different TFT colors:

| Activity | Display |
| -------- | ------- |
| INACTIVE | Red     |
| WALKING  | Blue    |
| RUNNING  | Green   |

---

# Complete System Flow

The complete health and activity monitoring concept can be represented as:

```text
                         SYSTEM START
                              │
                              ▼
                     Initialize RCC/GPIO
                              │
                ┌─────────────┴─────────────┐
                ▼                           ▼
            Initialize                  Initialize
             MPU6050                     MAX30102
                │                           │
                ▼                           ▼
        Read Acceleration              Acquire PPG
                │                           │
                ▼                           ▼
        Calculate Magnitude             Red / IR
                │                           │
                ▼                           ▼
        Apply Low-Pass Filter          VITALS Buffer
                │                           │
                ▼                    ┌──────┴──────┐
        Detect Step                   ▼             ▼
                │                   HR            SpO₂
                ▼                    │             │
      Classify Activity              └──────┬──────┘
                │                           │
          ┌─────┴─────┐                     │
          ▼           ▼                     │
       Walking     Running                  │
          │           │                     │
          └─────┬─────┘                     │
                ▼                           │
       Distance / Calories                  │
                │                           │
                └─────────────┬─────────────┘
                              ▼
                        TFT Dashboard
                              │
                              ▼
                       Heart Rate LEDs
                              │
                              ▼
                         USART Output
                              │
                              ▼
                         Repeat Loop
```

---

# MCAL Drivers

The Microcontroller Abstraction Layer provides low-level access to STM32 peripherals.

Current MCAL modules include:

### RCC

Responsible for:

* MCU clock initialization
* Peripheral clock enabling

### GPIO

Responsible for:

* Pin configuration
* Input/output control
* Alternate-function configuration
* Digital pin reading and writing

### I²C

Used for communication with I²C-based peripherals such as the MAX30102 and MPU6050.

### SPI

Provides serial communication for peripherals requiring SPI communication.

### SysTick

Used for:

* Timing
* Delays
* Periodic/single-shot callbacks
* Time measurement

### EXTI

Provides external interrupt configuration.

### NVIC

Manages interrupt configuration and interrupt enabling.

### SYSCFG

Provides system-level configuration required by external interrupt functionality.

---

# HAL Drivers

The Hardware Abstraction Layer provides drivers for external hardware modules.

Current modules include:

```text
MAX30102
MPU6050
TFT
```

The HAL layer hides hardware-specific implementation details from the application.

For example:

```text
Application
     │
     ▼
MAX30102 Driver
     │
     ▼
I2C Driver
     │
     ▼
STM32 I2C Peripheral
```

Similarly:

```text
Application
     │
     ▼
MPU6050 Driver
     │
     ▼
I2C / SPI Driver
     │
     ▼
STM32 Peripheral
```

---

# Application Modules

## Activity Tracker

The Activity Tracker is responsible for:

* Reading acceleration
* Calculating acceleration magnitude
* Filtering acceleration data
* Detecting steps
* Tracking peak acceleration
* Classifying walking/running
* Detecting inactivity
* Calculating distance
* Calculating calories

The main data structure is:

```text
ActivityTracker_t
```

which maintains the current activity and accumulated tracking values.

---

## VITALS

The VITALS module is responsible for processing MAX30102 samples.

It provides functionality for:

```text
MVITALS_vInit()
MVITALS_vAddSample()
MVITALS_u8IsBufferFull()
MVITALS_u8CalculateSpO2()
MVITALS_u8CalculateHR()
```

This separates sensor acquisition from health-data processing.

---

# USART Debugging

The MAX30102 health-monitoring subsystem provides serial output for debugging and monitoring.

Example output:

```text
MAX INIT...
MAX INIT OK

PRESS BUTTON TO START

BUTTON PRESSED
START VITALS

PPG ENABLE STATUS = 0
PA0 = 1

VITALS CALCULATION

==============================
       VITALS RESULT
==============================
SpO2 = 98 %
HEART RATE = 82 BPM
==============================
```

This makes it possible to monitor sensor operation and calculated results during development.

---

# Technologies Used

| Category             | Technology        |
| -------------------- | ----------------- |
| Microcontroller      | STM32F401CCU      |
| CPU Architecture     | ARM Cortex-M4     |
| Programming Language | Embedded C        |
| Development IDE      | STM32CubeIDE      |
| Architecture         | Bare-Metal        |
| Health Sensor        | MAX30102          |
| Motion Sensor        | MPU6050           |
| Display              | TFT               |
| Communication        | I²C / SPI / USART |
| Timing               | SysTick           |
| Interrupts           | EXTI / NVIC       |
| Debugging            | USART / ST-LINK   |

---

# Design Principles

The project was developed using several embedded-system design principles.

### Layered Architecture

The system separates:

```text
Application
     ↓
HAL
     ↓
MCAL
     ↓
Hardware
```

### Driver Abstraction

Application code does not directly manipulate peripheral registers.

### Modular Design

Each hardware module has its own driver.

### Reusable MCAL

Low-level drivers such as GPIO, I²C, SPI, SysTick, NVIC, and EXTI can be reused by multiple HAL modules.

### Separation of Sensor Acquisition and Processing

The MAX30102 driver is responsible for acquiring samples, while the VITALS module processes those samples.

Similarly, the MPU6050 driver provides acceleration data while the Activity Tracker performs motion analysis.

### Bare-Metal Development

The project focuses on direct STM32 peripheral configuration and custom driver implementation.

---

# Project Objectives

The project demonstrates the integration of multiple ARM embedded-systems concepts:

* ARM Cortex-M4 programming
* Embedded C
* STM32 peripheral programming
* Register-level driver development
* GPIO configuration
* I²C communication
* SPI communication
* USART communication
* Sensor interfacing
* PPG signal acquisition
* Heart-rate calculation
* SpO₂ calculation
* Accelerometer interfacing
* Digital signal filtering
* Peak detection
* Step detection
* Activity classification
* TFT display interfacing
* Interrupt handling
* SysTick timing
* Layered embedded software architecture

---

# Future Extensions

Possible extensions to the system include:

* 📶 ESP Wi-Fi health-data transmission
* 💾 EEPROM-based health-data storage
* 🔊 Audio feedback and health alerts
* 🔢 7-segment numerical display
* 💡 LED matrix visualization
* 🔋 Battery monitoring
* 💤 Low-power operating modes
* 📱 Mobile health-data dashboard
* ☁️ Cloud-based health-data storage
* 📈 Historical health and activity visualization
* 🔄 More advanced activity-classification algorithms
* ❤️ Improved PPG filtering and signal processing

---

# Disclaimer

This project is an **educational embedded-systems prototype**.

The heart-rate and SpO₂ measurements are intended for demonstration and embedded-systems development purposes and should **not be considered medical-grade measurements or used for medical diagnosis**.

---

# Project Status

🚧 **ARM Embedded Systems Graduation Project**

The project integrates health monitoring and activity tracking into an STM32-based embedded platform.

```text
                       STM32F401CCU
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
        ▼                   ▼                   ▼
    MAX30102             MPU6050              TFT
        │                   │                   │
        ▼                   ▼                   ▼
    Heart Rate         Step Counter       Live Dashboard
       SpO₂            Activity State
                          │
                    ┌─────┴─────┐
                    ▼           ▼
                 Distance    Calories
                          │
                          ▼
                    User Alerts
                          │
                          ▼
                    USART Output
```

The system demonstrates the complete embedded-software path from **sensor acquisition → signal processing → application logic → user interface** on an ARM Cortex-M4 microcontroller.
