#ifndef VIDEOPERVIEWWIDGET_H
#define VIDEOPERVIEWWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QAudioOutput>


class VideoPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPreviewWidget(QWidget *parent = nullptr);

    void setVideoFile(const QString &videoPath);
    void play();
    void pause();
    void stop();
    void setPosition(qint64 position);
    qint64 getDuration();
    qint64 getPosition();


private:

    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;


signals:
};

#endif // VIDEOPERVIEWWIDGET_H
