/* USER CODE BEGIN Header */
/** ************************************************************* *
 * @file        main.c
 * @brief       
 * 
 * @date        2021-05-16
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* ------------------------------------------------------------- --
	user includes
-- ------------------------------------------------------------- */
/* HW scheduler */
#include "jack.h"
#include "buzzer.h"
#include "mpu6050.h"
#include "ds3231.h"

/* SW scheduler */
#include "window.h"
#include "synchro.h"
#include "routines.h"

/* broadcast */
#include "broadcast.h"

/* recovery system */
#include "recsys.h"

/* configuration */
#include "config_file.h"


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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void IT_manager(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
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
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	
  //__HAL_TIM_CLEAR_FLAG(&htim2, TIM_SR_UIF);
  //__HAL_TIM_CLEAR_FLAG(&htim3, TIM_SR_UIF);

	/* scheduler init */
	synchro_init();
	window_init();

	/* hardware init */
	buzzer_init();
	jack_init();
	DS3231_Init();
	RECSYS_Init();
	//////////////////////////////////////////////////////////// REMOVE for debug ///////////////////////
    if(MPU6050_Init() == HAL_ERROR) SCB->AIRCR = 0x05fa0004;
	//MPU6050_Init();
  /* close the recovery system (blocking mode) */
    while(RECSYS_check_locked() != 0x03) {;}
    //////////////////////////////////////////////////////////// REMOVE for debug ///////////////////////

	/* set the scheduler in wait status */
	HAL_Delay(1000);
	phase_set(PHASE_WAIT);
	broadcast_uart_send(MSG_ID_phase_wait);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  	while (1)
  	{
		/* get new time reference for synchro */
		synchro_update();

		/* manage the IT received during the last cycle */
		IT_manager();

		/* select the scheduled routine */
		switch(phase_get())
		{
			case PHASE_WAIT :
			buzzer_update(BUZZER_WAIT_DELAY, BUZZER_WAIT_RATIO);
			routine_wait();
			break;

			case PHASE_ASCEND :
			buzzer_update(BUZZER_ASCEND_DELAY, BUZZER_ASCEND_RATIO);
			routine_ascend();
			break;

			case PHASE_DEPLOY :
			routine_deploy();
			break;

			case PHASE_DESCEND :
			buzzer_update(BUZZER_DESCEND_DELAY, BUZZER_DESCEND_RATIO);
			routine_descend();
			break;

			case PHASE_LANDED :
			buzzer_update(BUZZER_LANDED_DELAY, BUZZER_LANDED_RATIO);
			routine_landed();
			break;

			default :
			break;
		}

		/* wait the synchro time */
		synchro_wait();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/** ************************************************************* *
 * @brief       the timer IT callback manager is trigged by the
 * 				timer IT and set the associated flag to true
 * 
 * @param       htim 
 * ************************************************************* **/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2) IT_flag_window_relock();
	if (htim->Instance == TIM3) IT_flag_window_unlock();
}

/** ************************************************************* *
 * @brief       the GPIO IT callback manager is trigged by the
 * 				GPIO IT and set the associated flag to true
 * 
 * @param       GPIO_Pin 
 * ************************************************************* **/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Jack_Pin) IT_flag_jack();
}


/** ************************************************************* *
 * @brief       the IT manager check the IT flag and launch 
 * 				the functions associated
 * 
 * ************************************************************* **/
void IT_manager(void)
{
	if(get_winU_IT_flag()	== true) IT_routine_window_unlock();
	if(get_winR_IT_flag()	== true) IT_routine_window_relock();
	if(get_jack_IT_flag() 	== true) IT_routine_jack();
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
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
