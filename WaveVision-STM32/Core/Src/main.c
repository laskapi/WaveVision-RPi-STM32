/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "audio_engine.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AUDIO_BUF_SIZE 2048
#define HALF_BUF_SIZE (AUDIO_BUF_SIZE / 2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint16_t adc_buffer[AUDIO_BUF_SIZE];
volatile uint8_t data_ready = 0;
volatile uint16_t current_amp = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
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
    HAL_Init();

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    MX_ADC1_Init();

    /* USER CODE BEGIN 2 */
    AudioEngine_Init();
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, AUDIO_BUF_SIZE);
    HAL_TIM_Base_Start_IT(&htim2);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {



        if (data_ready)
        {
            AudioEngine_TransmitPacket(MSG_TYPE_AMPLITUDE, (uint8_t*)&current_amp, sizeof(current_amp));
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            data_ready = 0;
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
    // ... (Twoja konfiguracja zegara zostaje tutaj bez zmian) ...
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        current_amp = AudioEngine_CalculateAmplitude(&adc_buffer[0], HALF_BUF_SIZE);
        data_ready = 1;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        current_amp = AudioEngine_CalculateAmplitude(&adc_buffer[HALF_BUF_SIZE], HALF_BUF_SIZE);
        data_ready = 1;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}
