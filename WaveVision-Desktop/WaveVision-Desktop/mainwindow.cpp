#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    serial(new QSerialPort(this)) {
    ui->setupUi(this);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    connect(ui->btnConnect, &QPushButton::clicked, this,
            &MainWindow::onConnectPressed);
}

void MainWindow::onConnectPressed() {
    if (serial->isOpen()) {
        serial->close();
        ui->btnConnect->setText("Connect");
        return;
    }
    serial->setPortName("COM3");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        ui->btnConnect->setText("Disconnect");
        ui->consoleOut->appendPlainText("Connected to STM32");
    } else {
        ui->consoleOut->appendPlainText("Error: Could not open port.");
    }
}

// Dodaj zmienną globalną lub (lepiej) jako pole w klasie MainWindow w
// mainwindow.h: QByteArray buffer;

void MainWindow::readSerialData() {
    m_buffer.append(serial->readAll());
    while (m_buffer.size() >= 8) {
        int startIndex = m_buffer.indexOf("AV");
        if (startIndex == -1) {
            m_buffer.clear();
            break;
        }
        if (startIndex > 0) {
            m_buffer.remove(0, startIndex);
            continue;
        }
        uint16_t payloadSize = static_cast<uint8_t>(m_buffer[3]) |
                               (static_cast<uint8_t>(m_buffer[4]) << 8);
        int fullPacketSize = 5 + payloadSize + 1;
        if (m_buffer.size() >= fullPacketSize) {
            QByteArray packet = m_buffer.left(fullPacketSize);
            m_buffer.remove(0, fullPacketSize);
            processPacket(packet);
        } else {
            break;
        }
    }
}

void MainWindow::processPacket(const QByteArray &packet ){
    uint8_t type=packet[2];

    if(type==1){
            uint16_t amplitude=static_cast<uint8_t>(packet[5])|
                             (static_cast<uint8_t>(packet[6])<<8);
        ui->labelAmplitude->setText(QString("Amplituda: %1").arg(amplitude));
            qDebug()<< "Odebrano amplitudę:"<<amplitude;
        }
}
MainWindow::~MainWindow() { delete ui; }
