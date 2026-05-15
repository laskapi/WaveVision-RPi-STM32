#include "serialworker.h"
#include <QDebug>

SerialWorker::SerialWorker(QObject *parent) : QObject(parent) {
    m_serial = std::make_unique<QSerialPort>();
}

SerialWorker::~SerialWorker() {
    disconnectPort();
}

void SerialWorker::connectPort(const QString &portName, int baudRate) {
    if (m_serial->isOpen()) return;

    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);

    if (m_serial->open(QIODevice::ReadWrite)) {
        connect(m_serial.get(), &QSerialPort::readyRead, this, &SerialWorker::readData);
        emit connectionStatusChanged(true);
    } else {
        emit errorOccurred(m_serial->errorString());
    }
}

void SerialWorker::disconnectPort() {
    if (m_serial && m_serial->isOpen()) {
        m_serial->close();
        emit connectionStatusChanged(false);
    }
}

void SerialWorker::readData() {
    m_buffer.append(m_serial->readAll());

    while (true) {
        int index = m_buffer.indexOf("AV");
        if (index == -1) {
            // Keep last byte if it's 'A', might be start of header
            if (!m_buffer.isEmpty() && m_buffer.last() != 'A') m_buffer.clear();
            else if (m_buffer.size() > 1) m_buffer.remove(0, m_buffer.size() - 1);
            break;
        }

        if (index > 0) m_buffer.remove(0, index);
        if (m_buffer.size() < sizeof(AudioHeader)) break;

        const auto* hdr = reinterpret_cast<const AudioHeader*>(m_buffer.data());
        int totalSize = sizeof(AudioHeader) + hdr->payloadSize + 1;

        if (m_buffer.size() < totalSize) break;

        // XOR Checksum validation
        uint8_t checksum = hdr->packetType ^ (hdr->payloadSize & 0xFF) ^ (hdr->payloadSize >> 8);
        const uint8_t* payload = reinterpret_cast<const uint8_t*>(m_buffer.data() + sizeof(AudioHeader));
        for (int i = 0; i < hdr->payloadSize; ++i) checksum ^= payload[i];

        if (checksum != static_cast<uint8_t>(m_buffer[totalSize - 1])) {
            m_buffer.remove(0, 2);
            continue;
        }

        // Data dispatch
        int samplesCount = hdr->payloadSize / sizeof(int16_t);
        auto data = std::make_shared<QVector<int16_t>>(
            reinterpret_cast<const int16_t*>(payload),
            reinterpret_cast<const int16_t*>(payload) + samplesCount
            );

        emit samplesReady(data);
        m_buffer.remove(0, totalSize);
    }
}