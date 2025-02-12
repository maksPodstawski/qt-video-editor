#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../ui/ui_mainwindow.h"
#include "VideoData.h"
#include "VideoPerview.h"
#include "VideoTable.h"
#include "TimeLine.h"
#include "Editor.h"
#include "Options.h"
#include "ProjectSaver.h"
#include "ProjectLoader.h"
#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QTimer>
#include <QFileDialog>
#include <QAudioOutput>
#include <QDebug>
#include <QSlider>
#include <QMessageBox>
#include <QStyleFactory>
#include <QScrollArea>
#include <QProgressDialog>


QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_muteButton_clicked();
    void on_volumeChangeSlider_valueChanged(int value);
    void on_playPauseButton_clicked();
    void on_actionExport_triggered();
    void on_actionExport_options_triggered();
    void on_actionSave_Project_triggered();
    void on_actionOpen_Project_triggered();

private:
    Ui::MainWindow* ui;
    Options* options;
    VideoPreview* videoPreviewWidget;
    VideoTable* videoTable;
    TimeLine* timeLine;
    QTimer* positionUpdateTimer;

    bool isSliderBeingMoved = false;
    bool isProjectSaved;
    float previousVolume;

    void updateDurationLabel();
    void setupShortcuts();
    void setupIcons();

protected:
    void closeEvent(QCloseEvent* event) override;
};
#endif // MAINWINDOW_H
