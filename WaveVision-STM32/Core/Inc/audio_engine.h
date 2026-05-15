#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <stdint.h>


typedef enum {
    MSG_TYPE_AMPLITUDE = 1,
    MSG_TYPE_RAW_BUFFER = 2,
    MSG_TYPE_ERROR = 99
} MessageType;

typedef struct __attribute__((packed)) {
    uint8_t  sync[2];   // 'A', 'V'
    uint8_t  type;      // MessageType
    uint16_t length;    // Rozmiar payloadu w bajtach
} AV_Header;

typedef struct __attribute__((packed)) {
    AV_Header header;
    uint8_t   payload[];
} AV_Packet;

void AudioEngine_Init(void);
uint16_t AudioEngine_CalculateAmplitude(const uint16_t* buffer, uint16_t size);
void AudioEngine_TransmitPacket(MessageType type, uint8_t* data, uint16_t size);
void AudioEngine_SimulateSignal(uint16_t* buffer, uint16_t size);

#endif