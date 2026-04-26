//
// Created by Piotrek on 25.04.2026.
//

#include "audio_engine.h"

uint16_t adc_buffer[AUDIO_BUF_SIZE];

uint16_t AudioEngine_CalculateAmplitude(uint16_t* buffer_part, uint16_t size) {
    uint16_t min_v = 4095;
    uint16_t max_v = 0;

    for (int i = 0; i < size; i++) {
        if (buffer_part[i] < min_v) min_v = buffer_part[i];
        if (buffer_part[i] > max_v) max_v = buffer_part[i];
    }
    return max_v - min_v;
}