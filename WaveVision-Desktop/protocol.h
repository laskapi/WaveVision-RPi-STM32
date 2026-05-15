#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>

#pragma pack(push, 1)
/**
 * @brief Binary header for serial communication
 */
struct AudioHeader {
    char sync[2];        // 'A', 'V'
    uint8_t packetType;
    uint16_t payloadSize; // Size in bytes
};
#pragma pack(pop)

#endif // PROTOCOL_H