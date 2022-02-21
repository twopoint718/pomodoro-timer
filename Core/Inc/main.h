/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum _state {   // State description:
    START,              // State when first booted, init some peripherals here
    SHOW_STATS,         // "Splash screen" & display of current Pomodoro count
    POMODORO,           // During a 25-minute Pomodoro work session
    SHORT_BREAK,        // During a 5-minute break session
    LONG_BREAK,         // During a 15-minute break session
    TIMING,             // State common to all timing states
    ELAPSED,            // Timer has expired from one of the above sessions
    RESETTING,          // Waiting for device to prepare for next timing sess.
    WAIT                // Do-nothing state (TODO: could be low-power here?)
} state_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
state_t get_orientation(int16_t *pDataXYZ);

// STARTUP
// init(void); // inits all peripherals & changes to SHOW_STATS state

// TIMING
// start_timer(...) // enables interrupt/sets timer to given duration
// handle_times_up(...)

// ORIENTATION
// handle_orientation_change(...)

// BUTTON
// handle_reset_press(...)

// STATS
// stats_display(...)

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_RESET_Pin GPIO_PIN_2
#define LCD_RESET_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_3
#define LCD_DC_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define USER_BUTTON GPIO_PIN_13
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
