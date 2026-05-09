#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onConnectPressed();
    void readSerialData();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QByteArray m_buffer;
    void processPacket(const QByteArray &packet);
};
#endif // MAINWINDOW_H
