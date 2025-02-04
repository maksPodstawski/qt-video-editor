#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../ui/ui_mainwindow.h"
#include "AudioData.h"
#include "VideoData.h"
#include "VideoPerview.h"
#include "VideoTable.h"
#include "TimeLine.h"
#include "Editor.h"
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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionOpen_triggered();
    void on_muteButton_clicked();
    void on_volumeChangeSlider_valueChanged(int value);
    void on_playPauseButton_clicked();
    void on_actionExport_triggered();


private:
    Ui::MainWindow *ui;

    VideoPreview *videoPreviewWidget;
    VideoTable *videoTable;
    TimeLine *timeLine;
    QTimer *positionUpdateTimer;
    Editor *editor;

    bool isSliderBeingMoved = false;
    float previousVolume;

    void updateVideoTimeSlider();
    void updateDurationLabel();
    void setupShortcuts();
};
#endif // MAINWINDOW_H
