//
// Created by Piotrek on 25.04.2026.
//
#ifndef INC_AUDIO_ENGINE_H_
#define INC_AUDIO_ENGINE_H_

#include "main.h"

#define AUDIO_BUF_SIZE 2000
#define HALF_BUF_SIZE (AUDIO_BUF_SIZE / 2)

extern uint16_t adc_buffer[AUDIO_BUF_SIZE];

uint16_t AudioEngine_CalculateAmplitude(uint16_t* buffer_part, uint16_t size);

#endif