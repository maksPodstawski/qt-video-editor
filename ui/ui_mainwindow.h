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
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
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
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSlider *volumeChangeSlider;
    QVBoxLayout *verticalLayout_3;
    QPushButton *playPauseButton;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QLCDNumber *volumeLCD;
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
        timeLabel->setGeometry(QRect(1080, 430, 62, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        timeLabel->setFont(font);
        timeLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        groupBoxVideosTable = new QGroupBox(centralwidget);
        groupBoxVideosTable->setObjectName("groupBoxVideosTable");
        groupBoxVideosTable->setGeometry(QRect(20, 30, 371, 371));
        timeLineScrollArea = new QScrollArea(centralwidget);
        timeLineScrollArea->setObjectName("timeLineScrollArea");
        timeLineScrollArea->setGeometry(QRect(20, 460, 1251, 241));
        timeLineScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1249, 239));
        timeLineScrollArea->setWidget(scrollAreaWidgetContents);
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(1170, 160, 107, 135));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        volumeChangeSlider = new QSlider(widget);
        volumeChangeSlider->setObjectName("volumeChangeSlider");
        volumeChangeSlider->setStyleSheet(QString::fromUtf8(""));
        volumeChangeSlider->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(volumeChangeSlider);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        playPauseButton = new QPushButton(widget);
        playPauseButton->setObjectName("playPauseButton");

        verticalLayout_3->addWidget(playPauseButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        verticalLayout_3->addItem(verticalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        volumeLCD = new QLCDNumber(widget);
        volumeLCD->setObjectName("volumeLCD");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(volumeLCD->sizePolicy().hasHeightForWidth());
        volumeLCD->setSizePolicy(sizePolicy);
        volumeLCD->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        volumeLCD->setStyleSheet(QString::fromUtf8(""));
        volumeLCD->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);

        verticalLayout_2->addWidget(volumeLCD);

        muteButton = new QPushButton(widget);
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
