#include "mainwindow.h"
#include "serialworker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<AudioDataPtr>("AudioDataPtr");
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
