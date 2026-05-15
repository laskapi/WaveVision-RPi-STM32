#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "serialworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onConnectPressed();
    void updateGraph(AudioDataPtr samples);

private:
    Ui::MainWindow *ui;
    QThread *workerThread;
    SerialWorker *worker;
};
#endif