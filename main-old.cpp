#include <iostream>
#include <vector>
#include <cstdint>
#include "audio_engine.h"

class PacketParser {
public:
    enum class State { WAIT_A, WAIT_V, GET_TYPE, GET_LEN_L, GET_LEN_H, GET_PAYLOAD, VERIFY };

private:
    State currentState = State::WAIT_A;
    uint8_t msgType = 0;
    uint16_t expectedLen = 0;
    std::vector<uint8_t> payload;
    uint8_t calculatedXor = 0;
    uint8_t tempLenL = 0;

public:
    void parseByte(uint8_t byte) {
        switch (currentState) {
            case State::WAIT_A:
                if (byte == 'A') currentState = State::WAIT_V;
                break;

            case State::WAIT_V:
                if (byte == 'V') currentState = State::GET_TYPE;
                else currentState = State::WAIT_A;
                break;

            case State::GET_TYPE:
                msgType = byte;
                calculatedXor = byte; // Start checksum with type
                currentState = State::GET_LEN_L;
                break;

            case State::GET_LEN_L:
                tempLenL = byte;
                calculatedXor ^= byte;
                currentState = State::GET_LEN_H;
                break;

            case State::GET_LEN_H:
                expectedLen = (byte << 8) | tempLenL;
                calculatedXor ^= byte;
                payload.clear();
                if (expectedLen > 0) currentState = State::GET_PAYLOAD;
                else currentState = State::VERIFY;
                break;

            case State::GET_PAYLOAD:
                payload.push_back(byte);
                calculatedXor ^= byte;
                if (payload.size() == expectedLen) currentState = State::VERIFY;
                break;

            case State::VERIFY:
                if (byte == calculatedXor) {
                    processPacket();
                } else {
                    std::cerr << "Checksum Error!" << std::endl;
                }
                currentState = State::WAIT_A;
                break;
        }
    }

private:
    void processPacket() {
        if (msgType == MSG_TYPE_AMPLITUDE) {

            uint16_t amplitude = (payload[1] << 8) | payload[0];
            std::cout << "[SUCCESS] Received Amplitude: " << amplitude << std::endl;
        }
    }
};

int main() {
    PacketParser parser;

    // Simulated data stream from STM32:
    // 'A', 'V', Type=1, Len=2 (0x02, 0x00), Data=500 (0xF4, 0x01), Checksum
    // Checksum: 1 ^ 2 ^ 0 ^ 0xF4 ^ 1 = 248 (0xF8)
    std::vector<uint8_t> mockStream = {'A', 'V', 1, 2, 0, 0xF4, 1, 0xF6};

    std::cout << "Starting parser test..." << std::endl;
    for (uint8_t b : mockStream) {
        parser.parseByte(b);
    }

    return 0;
}