/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l475e_iot01_accelero.h"
#include "draw.h"
#include "pcd8544.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim2;                 // Timer 2 handle

static volatile state_t State;                  // initial state is START
static volatile uint8_t num_pomodoros = 0;      // current number of pomodoros
static volatile uint32_t ticks_remaining = 0;   // how long until timer elapses
static bool pomodoro_started = false;  // Keep track of poms
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static int16_t pDataXYZ[3]; // accelerometer data
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  const char msg[]         = "Pomodoros:";
  const char pom_lbl[]     = "POMODORO";
  const char sbreak_lbl[]  = "S. BREAK";
  const char lbreak_lbl[]  = "L. BREAK";
  const char elapsed_lbl[] = "DONE!";
  state_t orientation_state;        // The current timing/orientation state

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_I2C2_Init();
  MX_UART4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  BSP_ACCELERO_Init();
  LCD_begin(&hspi1, GPIOA, LCD_DC_Pin, LCD_RESET_Pin, 40, 0x04);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  State = START;
  while (1) {
      switch(State) {
      case START:
          // TODO: start/init PWM, start/init button (GPIO)
          State = SHOW_STATS;
          break;
      case SHOW_STATS:
          LCD_clearDisplay();                   // clear display
          draw_string(0, 0, msg, sizeof(msg));  // "Pomodoros:"
          draw_number(0, 8, num_pomodoros);     // NUM
          LCD_display();                        // write to display
          State = WAIT;
          HAL_Delay(1000);                      // Hold for accel to settle
          break;
      case WAIT:
          BSP_ACCELERO_AccGetXYZ(pDataXYZ);     // read accelerometer
          orientation_state = get_orientation(pDataXYZ);
          State = orientation_state;
          break;
      case POMODORO:
          ticks_remaining = 15;                 // set timer for 25 minutes
          pomodoro_started = true;
          LCD_clearDisplay();
          draw_string(0, 0, pom_lbl, sizeof(pom_lbl));
          HAL_TIM_Base_MspInit(&htim2);         // enable timer interrupt
          State = TIMING;
          break;
      case LONG_BREAK:
          ticks_remaining = 9;                  // set timer for 15 minutes
          LCD_clearDisplay();
          draw_string(0, 0, lbreak_lbl, sizeof(lbreak_lbl));
          HAL_TIM_Base_MspInit(&htim2);         // enable timer interrupt
          State = TIMING;
          break;
      case SHORT_BREAK:
          ticks_remaining = 3;                  // set timer for 5 minutes
          LCD_clearDisplay();
          draw_string(0, 0, sbreak_lbl, sizeof(sbreak_lbl));
          HAL_TIM_Base_MspInit(&htim2);         // enable timer interrupt
          State = TIMING;
          break;
      case TIMING:
          draw_clear_rect(0, 24, LCDWIDTH, 8);
          draw_number16(0, 24, ticks_remaining);
          LCD_display();
          break;
      case ELAPSED:
          HAL_TIM_Base_MspDeInit(&htim2);       // disable timer interrupt
          if (pomodoro_started) {               // IF we finished a pomodoro
              num_pomodoros++;                  // Increment statistics
              pomodoro_started = false;         // no longer in a pomodoro
          }
          LCD_clearDisplay();
          draw_string(0, 0, elapsed_lbl, sizeof(elapsed_lbl));
          LCD_display();
          State = RESETTING;
          break;
      case RESETTING:
          BSP_ACCELERO_AccGetXYZ(pDataXYZ);
          if (get_orientation(pDataXYZ) == WAIT) { // stay here until timer is
              State = SHOW_STATS;                  //   re-oriented, show counts
          }
          break;
      default:
          // Something has gone wrong
          Error_Handler();
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*******************************************************************************
 * Application-level tick callback. This should be about 1 actual second. Each
 * time we get here we should decide if a pomodoro counter has elapsed or else
 * continue to tick down.
 ******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (ticks_remaining == 0) {
        State = ELAPSED;
    } else {
        ticks_remaining--;
    }
}

/*******************************************************************************
 * Determine orientation for the board based on the current accelerometer
 * reading. Note that positive Z is "down" as viewed from the front/face.
 ┌─────────────┐
 │     +X      │
 │      ▲      │
 │      │      │
 │      │      │
 │+Y◀───◎      │
 │     +Z      │
 │ (into page) │
 │          ┌──┴──┐
 │          │ USB │
 │          └──┬──┘
 └─────────────┘
 The value returned is in mg, (i.e. 1/1000 of g). Full-scale is about 1,000 on
 any axis. This function returns one of the timing states to transition to or
 else to wait.
*******************************************************************************/
state_t get_orientation(int16_t *pDataXYZ) {
    int16_t x = pDataXYZ[0];
    int16_t y = pDataXYZ[1];
    int16_t z = pDataXYZ[2];
    if (x < -800 && abs(y) < 100 && abs(z) < 100) // (all neg X direction)
        return POMODORO;
    if (y > 800 && x > 450 && abs(z) < 100)       // y>1000*cos(30), x>1000*sin(30)
        return LONG_BREAK;
    if (y < -800 && x > 450 && abs(z) < 100)      // (^^ reflected over x-axis)
        return SHORT_BREAK;
    // In positive z direction, or indeterminate
    return WAIT;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while(1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

