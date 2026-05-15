#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    workerThread = new QThread(this);
    worker = new SerialWorker();
    worker->moveToThread(workerThread);

    // Business logic connections
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectPressed);
    connect(worker, &SerialWorker::samplesReady, this, &MainWindow::updateGraph);

    // Status and error handling
    connect(worker, &SerialWorker::connectionStatusChanged, this, [this](bool connected){
        ui->btnConnect->setText(connected ? "Disconnect" : "Connect");
        ui->consoleOut->appendPlainText(connected ? "System Online" : "System Offline");
    });

    connect(worker, &SerialWorker::errorOccurred, this, [this](const QString &err){
        ui->consoleOut->appendPlainText("Critical Error: " + err);
    });

    // Cleanup
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

    workerThread->start();
}

void MainWindow::onConnectPressed() {
    if (ui->btnConnect->text() == "Connect") {
        QMetaObject::invokeMethod(worker, "connectPort",
                                  Q_ARG(QString, "COM3"),
                                  Q_ARG(int, 115200));
    } else {
        QMetaObject::invokeMethod(worker, "disconnectPort");
    }
}

void MainWindow::updateGraph(AudioDataPtr samples) {
    ui->waveformWidget->setSamples(samples);
}

MainWindow::~MainWindow() {
    workerThread->quit();
    workerThread->wait();
    delete ui;
}