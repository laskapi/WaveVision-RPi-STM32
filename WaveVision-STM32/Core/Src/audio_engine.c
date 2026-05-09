#include "audio_engine.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;

void AudioEngine_Init(void) {
}

uint16_t AudioEngine_CalculateAmplitude(uint16_t* buffer, uint16_t size) {
    uint16_t min = 4095;
    uint16_t max = 0;

    for (uint16_t i = 0; i < size; i++) {
        if (buffer[i] < min) min = buffer[i];
        if (buffer[i] > max) max = buffer[i];
    }
    return (max - min);
}

void AudioEngine_TransmitPacket(MessageType type, uint8_t* data, uint16_t size) {
    AV_Header hdr;
    hdr.sync[0] = 'A';
    hdr.sync[1] = 'V';
    hdr.type = (uint8_t)type;
    hdr.length = size;

    uint8_t checksum = hdr.type ^ (uint8_t)(size & 0xFF) ^ (uint8_t)(size >> 8);
    for (uint16_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    /*HAL_StatusTypeDef res;
    res = HAL_UART_Transmit(&huart2, (uint8_t*)&hdr, sizeof(AV_Header), 100);
    if(res == HAL_OK) {
        res = HAL_UART_Transmit(&huart2, data, size, 100);
    }
    if(res == HAL_OK) {
        HAL_UART_Transmit(&huart2, &checksum, 1, 100);
    }*/
    HAL_UART_Transmit(&huart2, (uint8_t*)&hdr, sizeof(AV_Header), 100);
    HAL_UART_Transmit(&huart2, data, size, 100);
    HAL_UART_Transmit(&huart2, &checksum, 1, 100);
}

void AudioEngine_SimulateSignal(uint16_t* buffer, uint16_t size) {
    static uint32_t counter = 0;
    for (int i = 0; i < size; i++) {
        buffer[i] = 2048 + ((counter + i) % 500) - 250;
    }
    counter += 10;
}