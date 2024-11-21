#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QTimer>
#include "videoperviewwidget.h"
#include "videotablewidget.h"
#include "timelinewidget.h"
#include "AudioData.h"
#include "VideoData.h"

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
    void on_playButton_clicked();
    void on_pauseButton_clicked();

private:
    Ui::MainWindow *ui;

    VideoPreviewWidget *videoPreviewWidget;
    VideoTableWidget *videoTable;
    TimelineWidget *timeLine;
    QTimer *positionUpdateTimer;

    bool isSliderBeingMoved = false;
    float previousVolume;

    void updateVideoTimeSlider();
    void updateDurationLabel();
};
#endif // MAINWINDOW_H
