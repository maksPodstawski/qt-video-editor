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

    void play();
    void pause();
    void stop();
    void setVideoFile(const QString &videoPath);
    void setPosition(qint64 position);
    void setVolume(float volume);
    qint64 getDuration();
    qint64 getPosition();
    float getVolume() const;

private:

    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;


signals:
};

#endif // VIDEOPERVIEWWIDGET_H
