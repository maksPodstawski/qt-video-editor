#include "../include/mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("../../resources/icons/programicon.png"));

    QFile styleFile("../../styles/app.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());

    app.setStyleSheet(styleSheet);

    w.show();
    return app.exec();
}

