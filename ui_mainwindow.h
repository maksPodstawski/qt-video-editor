/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QWidget *centralwidget;
    QGroupBox *groupBoxVideo;
    QSlider *volumeChangeSlider;
    QPushButton *playButton;
    QPushButton *muteButton;
    QLCDNumber *volumeLCD;
    QPushButton *pauseButton;
    QLabel *timeLabel;
    QSlider *previewVideoTimeSlider;
    QGroupBox *groupBoxVideosTable;
    QGraphicsView *graphicsViewTimeLine;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1304, 669);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBoxVideo = new QGroupBox(centralwidget);
        groupBoxVideo->setObjectName("groupBoxVideo");
        groupBoxVideo->setGeometry(QRect(620, 31, 501, 331));
        volumeChangeSlider = new QSlider(centralwidget);
        volumeChangeSlider->setObjectName("volumeChangeSlider");
        volumeChangeSlider->setGeometry(QRect(1170, 151, 16, 160));
        volumeChangeSlider->setOrientation(Qt::Orientation::Vertical);
        playButton = new QPushButton(centralwidget);
        playButton->setObjectName("playButton");
        playButton->setGeometry(QRect(520, 180, 80, 24));
        muteButton = new QPushButton(centralwidget);
        muteButton->setObjectName("muteButton");
        muteButton->setGeometry(QRect(1150, 321, 61, 41));
        volumeLCD = new QLCDNumber(centralwidget);
        volumeLCD->setObjectName("volumeLCD");
        volumeLCD->setGeometry(QRect(1200, 221, 64, 23));
        volumeLCD->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
        pauseButton = new QPushButton(centralwidget);
        pauseButton->setObjectName("pauseButton");
        pauseButton->setGeometry(QRect(520, 240, 80, 24));
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setGeometry(QRect(470, 380, 131, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        timeLabel->setFont(font);
        timeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        previewVideoTimeSlider = new QSlider(centralwidget);
        previewVideoTimeSlider->setObjectName("previewVideoTimeSlider");
        previewVideoTimeSlider->setGeometry(QRect(610, 380, 511, 31));
        previewVideoTimeSlider->setOrientation(Qt::Orientation::Horizontal);
        previewVideoTimeSlider->setTickPosition(QSlider::TickPosition::TicksBothSides);
        groupBoxVideosTable = new QGroupBox(centralwidget);
        groupBoxVideosTable->setObjectName("groupBoxVideosTable");
        groupBoxVideosTable->setGeometry(QRect(30, 70, 421, 311));
        graphicsViewTimeLine = new QGraphicsView(centralwidget);
        graphicsViewTimeLine->setObjectName("graphicsViewTimeLine");
        graphicsViewTimeLine->setGeometry(QRect(70, 430, 1131, 192));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1304, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
        groupBoxVideo->setTitle(QString());
        playButton->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        muteButton->setText(QCoreApplication::translate("MainWindow", "Mute", nullptr));
        pauseButton->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        groupBoxVideosTable->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
