#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "audio_engine.h"

/* Buffers */
#define AUDIO_BUF_SIZE 2048
#define HALF_BUF_SIZE (AUDIO_BUF_SIZE / 2)
uint16_t adc_buffer[AUDIO_BUF_SIZE];

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    MX_ADC1_Init();

    AudioEngine_Init();

    /* Start background acquisition */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, AUDIO_BUF_SIZE);
    HAL_TIM_Base_Start_IT(&htim2);

    while (1)
    {
        /* Driven by interrupts */
    }
}

/* Process first 50% of buffer */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        uint16_t amp = AudioEngine_CalculateAmplitude(&adc_buffer[0], HALF_BUF_SIZE);
        AudioEngine_TransmitPacket(MSG_TYPE_AMPLITUDE, (uint8_t*)&amp, sizeof(amp));

        /* Status LED */
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (amp > 500) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

/* Process second 50% of buffer */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        uint16_t amp = AudioEngine_CalculateAmplitude(&adc_buffer[HALF_BUF_SIZE], HALF_BUF_SIZE);
        AudioEngine_TransmitPacket(MSG_TYPE_AMPLITUDE, (uint8_t*)&amp, sizeof(amp));
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}