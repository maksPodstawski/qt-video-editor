#include "../include/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLocale::setDefault(QLocale(QLocale::Polish, QLocale::Poland));
    a.setStyle("Fusion");
    MainWindow w;
    w.show();
    return a.exec();
}
