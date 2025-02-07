#ifndef VIDEOPERVIEW_H
#define VIDEOPERVIEW_H

#include "VideoData.h"
#include "TimeLine.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QMimeData>
#include "CutLeftOperation.h"
#include "CutRightOperation.h"


class VideoPreview : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPreview(TimeLine *timeLine, QWidget *parent = nullptr);

    TimeLine *timeLine;

    void updateIndicatorPosition(qint64 position);
    void moveIndicator();

    void play();
    void pause();
    void stop();

    void setPosition(qint64 position);
    void setVolume(float volume);
    void setVideoFiles(const QList<QString> &files);

    float getVolume() const;
    qint64 getDuration();
    qint64 getPosition();
    QMediaPlayer::PlaybackState getPlaybackState() const;
    QMediaPlayer* getMediaPlayer() const;

    void playVideo(const QString &filePath);

    signals:
    void playPauseButtonTextChanged(const QString &text);

private:
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;
    QList<QTimer*> indicatorTimers;
    void determineStartEndTimes(const VideoData &videoData, qreal &startTime, qreal &endTime);
    int currentVideoIndex;

    qint64 m_iVideoPosition;
    qint64 pausedPosition;


private slots:
    void playNextVideo();
    void updatePlayer();
};

#endif // VIDEOPERVIEW_H
