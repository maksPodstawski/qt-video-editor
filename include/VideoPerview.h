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
    explicit VideoPreview(TimeLine* timeLine, QWidget* parent = nullptr);

    void setPosition(qint64 position);
    void setVolume(float volume);

    float getVolume() const;
    qint64 getDuration() const;
    qint64 getPosition() const;
    QMediaPlayer::PlaybackState getPlaybackState() const;

    void updateIndicatorPosition(qint64 position);
    void moveIndicator();
    void play();
    void pause();
    void stop();

    TimeLine* timeLine;

signals:
    void playPauseButtonTextChanged(const QString& text);
    void playbackFinished();

public slots:
    void resetVideoPlayer();
    void resetIndicatorPosition();
    void clearMediaPlayer();

private:
    QMediaPlayer* mediaPlayer;
    QVideoWidget* videoWidget;
    QAudioOutput* audioOutput;
    QList<QTimer*> indicatorTimers;
    int currentVideoIndex;
    qint64 currentVideoPosition;
    qint64 m_iVideoPosition;

    void determineStartEndTimes(const VideoData& videoData, qreal& startTime, qreal& endTime);

private slots:
    void playNextVideo();
    void updatePlayer();
    void onIndicatorPositionChanged();
};

#endif // VIDEOPERVIEW_H
