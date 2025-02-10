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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExport;
    QAction *actionExport_options;
    QAction *actionSave_Project;
    QAction *actionOpen_Project;
    QWidget *centralwidget;
    QGroupBox *groupBoxVideo;
    QSlider *volumeChangeSlider;
    QPushButton *playPauseButton;
    QPushButton *muteButton;
    QLCDNumber *volumeLCD;
    QLabel *timeLabel;
    QGroupBox *groupBoxVideosTable;
    QScrollArea *timeLineScrollArea;
    QWidget *scrollAreaWidgetContents;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuOptions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1297, 747);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName("actionExport");
        actionExport_options = new QAction(MainWindow);
        actionExport_options->setObjectName("actionExport_options");
        actionSave_Project = new QAction(MainWindow);
        actionSave_Project->setObjectName("actionSave_Project");
        actionOpen_Project = new QAction(MainWindow);
        actionOpen_Project->setObjectName("actionOpen_Project");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBoxVideo = new QGroupBox(centralwidget);
        groupBoxVideo->setObjectName("groupBoxVideo");
        groupBoxVideo->setGeometry(QRect(430, 30, 701, 371));
        volumeChangeSlider = new QSlider(centralwidget);
        volumeChangeSlider->setObjectName("volumeChangeSlider");
        volumeChangeSlider->setGeometry(QRect(1150, 200, 16, 160));
        playPauseButton = new QPushButton(centralwidget);
        playPauseButton->setObjectName("playPauseButton");
        playPauseButton->setGeometry(QRect(1150, 130, 80, 24));
        muteButton = new QPushButton(centralwidget);
        muteButton->setObjectName("muteButton");
        muteButton->setGeometry(QRect(1180, 310, 61, 41));
        volumeLCD = new QLCDNumber(centralwidget);
        volumeLCD->setObjectName("volumeLCD");
        volumeLCD->setGeometry(QRect(1180, 240, 61, 31));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(volumeLCD->sizePolicy().hasHeightForWidth());
        volumeLCD->setSizePolicy(sizePolicy);
        volumeLCD->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        volumeLCD->setStyleSheet(QString::fromUtf8(""));
        volumeLCD->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setGeometry(QRect(1140, 410, 141, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        timeLabel->setFont(font);
        groupBoxVideosTable = new QGroupBox(centralwidget);
        groupBoxVideosTable->setObjectName("groupBoxVideosTable");
        groupBoxVideosTable->setGeometry(QRect(30, 30, 371, 371));
        timeLineScrollArea = new QScrollArea(centralwidget);
        timeLineScrollArea->setObjectName("timeLineScrollArea");
        timeLineScrollArea->setGeometry(QRect(10, 460, 1241, 221));
        timeLineScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1239, 219));
        timeLineScrollArea->setWidget(scrollAreaWidgetContents);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1297, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName("menuOptions");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menuFile->addAction(actionOpen_Project);
        menuFile->addAction(actionSave_Project);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExport);
        menuOptions->addAction(actionExport_options);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+I", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExport->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
#if QT_CONFIG(shortcut)
        actionExport->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExport_options->setText(QCoreApplication::translate("MainWindow", "Export options", nullptr));
        actionSave_Project->setText(QCoreApplication::translate("MainWindow", "Save Project", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_Project->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen_Project->setText(QCoreApplication::translate("MainWindow", "Open Project", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen_Project->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        groupBoxVideo->setTitle(QString());
        playPauseButton->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        muteButton->setText(QCoreApplication::translate("MainWindow", "Mute", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        groupBoxVideosTable->setTitle(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuOptions->setTitle(QCoreApplication::translate("MainWindow", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
