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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
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
    QLabel *timeLabel;
    QGroupBox *groupBoxVideosTable;
    QScrollArea *timeLineScrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSlider *volumeChangeSlider;
    QVBoxLayout *verticalLayout_3;
    QPushButton *playPauseButton;
    QVBoxLayout *verticalLayout_2;
    QLabel *volumeLabel;
    QPushButton *muteButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuOptions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1299, 747);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 0));
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
        groupBoxVideo->setGeometry(QRect(400, 30, 751, 371));
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setGeometry(QRect(20, 430, 1251, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        timeLabel->setFont(font);
        timeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBoxVideosTable = new QGroupBox(centralwidget);
        groupBoxVideosTable->setObjectName("groupBoxVideosTable");
        groupBoxVideosTable->setGeometry(QRect(20, 30, 371, 371));
        timeLineScrollArea = new QScrollArea(centralwidget);
        timeLineScrollArea->setObjectName("timeLineScrollArea");
        timeLineScrollArea->setGeometry(QRect(20, 460, 1250, 200));
        timeLineScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1250, 200));
        timeLineScrollArea->setWidget(scrollAreaWidgetContents);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(1170, 160, 107, 135));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        volumeChangeSlider = new QSlider(layoutWidget);
        volumeChangeSlider->setObjectName("volumeChangeSlider");
        volumeChangeSlider->setStyleSheet(QString::fromUtf8(""));
        volumeChangeSlider->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(volumeChangeSlider);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        playPauseButton = new QPushButton(layoutWidget);
        playPauseButton->setObjectName("playPauseButton");

        verticalLayout_3->addWidget(playPauseButton);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        volumeLabel = new QLabel(layoutWidget);
        volumeLabel->setObjectName("volumeLabel");

        verticalLayout_2->addWidget(volumeLabel);

        muteButton = new QPushButton(layoutWidget);
        muteButton->setObjectName("muteButton");

        verticalLayout_2->addWidget(muteButton);


        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout->addLayout(verticalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1299, 21));
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
        timeLabel->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        groupBoxVideosTable->setTitle(QString());
        playPauseButton->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        volumeLabel->setText(QString());
        muteButton->setText(QCoreApplication::translate("MainWindow", "Mute", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuOptions->setTitle(QCoreApplication::translate("MainWindow", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
