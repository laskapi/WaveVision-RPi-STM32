#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include "protocol.h"
#include <QObject>
#include <QSerialPort>
#include <QVector>
#include <memory>

using AudioDataPtr = std::shared_ptr<QVector<int16_t>>;

class SerialWorker : public QObject {
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

public slots:
    void connectPort(const QString &portName, int baudRate);
    void disconnectPort();

signals:
    void errorOccurred(const QString &s);
    void connectionStatusChanged(bool connected);
    void samplesReady(AudioDataPtr samples);

private slots:
    void readData();

private:
    std::unique_ptr<QSerialPort> m_serial;
    QByteArray m_buffer;
};
#endif