#ifndef VIDEOPERVIEW_H
#define VIDEOPERVIEW_H

#include "VideoData.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QMimeData>


class VideoPreview : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPreview(QWidget *parent = nullptr);

    void play();
    void pause();
    void stop();
    void setPosition(qint64 position);
    void setVolume(float volume);
    void setVideoFiles(const QList<QString> &files);
    float getVolume() const;
    qint64 getDuration();
    qint64 getPosition();

private:
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;
    QList<QString> videoFiles;
    int currentVideoIndex;

private slots:
    void playNextVideo();

};

#endif // VIDEOPERVIEW_H
