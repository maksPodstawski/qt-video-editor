#include "../include/VideoPerview.h"


VideoPreview::VideoPreview(TimeLine* timeLine, QWidget* parent)
    : QWidget{parent},
      mediaPlayer(new QMediaPlayer(this)),
      videoWidget(new QVideoWidget(this)),
      audioOutput(new QAudioOutput(this)),
      timeLine(timeLine),
      currentVideoIndex(0),
      m_iVideoPosition(0)
{
    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setAudioOutput(audioOutput);

    audioOutput->setVolume(0.4);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    setLayout(layout);

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::EndOfMedia)
        {
            playNextVideo();
        }
    });

    connect(timeLine->indicator, &Indicator::indicatorReleased, this, &VideoPreview::onIndicatorPositionChanged);
    connect(timeLine, &TimeLine::resetIndicatorPosition, this, &VideoPreview::resetIndicatorPosition);
    connect(timeLine, &TimeLine::clearMediaPlayer, this, &VideoPreview::clearMediaPlayer);
}

void VideoPreview::updateIndicatorPosition(qint64 position)
{
    const auto& videoList = timeLine->getVideoList();
    if (videoList.isEmpty())
    {
        qWarning() << "Video list is empty.";
        return;
    }

    const VideoData& currentVideo = videoList[currentVideoIndex];
    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);

    qint64 duration = (endTime == -1)
                          ? mediaPlayer->duration() - startTime
                          : static_cast<qint64>(endTime - startTime);

    if (duration <= 0)
    {
        qWarning() << "Invalid video duration.";
        return;
    }

    int startX = currentVideo.getRect().left();
    int endX = currentVideo.getRect().right();
    int totalDistance = endX - startX;

    int currentX = startX + ((position - static_cast<qint64>(startTime)) * totalDistance / duration);
    currentX = qBound(startX, currentX, endX);

    timeLine->indicator->move(currentX, timeLine->indicator->y());
    timeLine->update();
}


void VideoPreview::play()
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PausedState)
    {
        int indicatorX = timeLine->indicator->x();
        const auto& videoList = timeLine->getVideoList();
        if (videoList.isEmpty())
        {
            qWarning() << "No videos to play.";
            return;
        }

        const VideoData& currentVideo = videoList[currentVideoIndex];
        int startX = currentVideo.getRect().left();
        double timePerUnit = timeLine->getTimePerUnit();

        qreal startTime, endTime;
        determineStartEndTimes(currentVideo, startTime, endTime);
        qreal newPosition = (indicatorX - startX) * timePerUnit + startTime * 1000;

        mediaPlayer->setPosition(currentVideoPosition);
        qDebug() << "Resuming playback from new position:" << currentVideoPosition;
        mediaPlayer->play();
        return;
    }

    const auto& videoList = timeLine->getVideoList();

    if (videoList.isEmpty())
    {
        qWarning() << "No videos to stop.";
        return;
    }

    currentVideoIndex = timeLine->getCurrentVideoIndexIndicator();
    qDebug() << "Current video index from timeline:" << currentVideoIndex;
    updatePlayer();

    int indicatorX = timeLine->indicator->x();
    const VideoData& currentVideo = videoList[currentVideoIndex];
    int startX = currentVideo.getRect().left();
    double timePerUnit = timeLine->getTimePerUnit();

    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);

    qreal newPosition = (indicatorX - startX) * timePerUnit + startTime * 1000;

    qDebug() << "Starting playback from position:" << newPosition;

    mediaPlayer->setPosition(currentVideoPosition);
    mediaPlayer->play();
}

void VideoPreview::stop()
{
    qDebug() << "stop!!!!!!!!";
    currentVideoPosition = 0;
    currentVideoIndex = 0;
    mediaPlayer->stop();
}

void VideoPreview::resetVideoPlayer()
{
    stop();
}

void VideoPreview::resetIndicatorPosition()
{
    currentVideoPosition = 0;
    currentVideoIndex = 0;
    qDebug() << "reset Indicator";
}

void VideoPreview::pause()
{
    const auto& videoList = timeLine->getVideoList();
    if (videoList.isEmpty())
    {
        qWarning() << "No videos to stop.";
        return;
    }


    currentVideoPosition = mediaPlayer->position();
    mediaPlayer->pause();
}

void VideoPreview::setPosition(qint64 position)
{
    mediaPlayer->setPosition(position);
}

qint64 VideoPreview::getDuration() const
{
    return mediaPlayer->duration();
}

qint64 VideoPreview::getPosition() const
{
    return mediaPlayer->position();
}

QMediaPlayer::PlaybackState VideoPreview::getPlaybackState() const
{
    return mediaPlayer->playbackState();
}

void VideoPreview::setVolume(float volume)
{
    audioOutput->setVolume(volume);
}

float VideoPreview::getVolume() const
{
    return audioOutput->volume();
}

void VideoPreview::updatePlayer()
{
    const auto& videoList = timeLine->getVideoList();
    if (currentVideoIndex >= videoList.size())
    {
        qWarning() << "Invalid video index:" << currentVideoIndex;
        return;
    }

    const VideoData& currentVideo = videoList[currentVideoIndex];
    mediaPlayer->setSource(QUrl::fromLocalFile(currentVideo.getFilePath()));
    qDebug() << "currentVideoIndex" << currentVideoIndex;

    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);

    m_iVideoPosition = static_cast<qint64>(startTime);

    disconnect(mediaPlayer, &QMediaPlayer::positionChanged, nullptr, nullptr);
    disconnect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, nullptr, nullptr);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position)
    {
        updateIndicatorPosition(position);
    });

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::EndOfMedia)
        {
            qDebug() << "End of media reached for video index:" << currentVideoIndex;
            playNextVideo();
        }
    });

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::BufferedMedia)
        {
            qDebug() << "BufferedMedia status reached. Setting position to:" << m_iVideoPosition;
            qDebug() << "Current Video Position. Setting position to:" << currentVideoPosition;

            qint64 time; //= (m_iVideoPosition > currentVideoPosition) ? m_iVideoPosition : currentVideoPosition;

            if(m_iVideoPosition > currentVideoPosition || currentVideoPosition > getDuration())
            {
                time = m_iVideoPosition;
            }
            else
            {
                time = currentVideoPosition;
            }
            mediaPlayer->setPosition(time);
        }
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this, endTime](qint64 position)
    {
        if (endTime != -1 && position >= static_cast<qint64>(endTime))
        {
            mediaPlayer->stop();
            playNextVideo();
        }
    });


}

void VideoPreview::onIndicatorPositionChanged()
{
    const auto& videoList = timeLine->getVideoList();
    if (videoList.isEmpty())
    {
        qWarning() << "No videos to stop.";
        return;
    }
    const VideoData* currentVideo = timeLine->getCurrentIndicatorVideo();
    if (currentVideo)
    {
        this->currentVideoIndex = timeLine->getCurrentVideoIndexIndicator();

        if (mediaPlayer->source() != QUrl::fromLocalFile(currentVideo->getFilePath()))
        {
            this->mediaPlayer->setSource(QUrl::fromLocalFile(currentVideo->getFilePath()));
        }

        qreal startTime, endTime;
        determineStartEndTimes(*currentVideo, startTime, endTime);

        int indicatorX = timeLine->indicator->x();
        int startX = currentVideo->getRect().left();
        double timePerUnit = timeLine->getTimePerUnit();
        qreal newPosition = (indicatorX - startX) * timePerUnit + startTime * 1000;

        currentVideoPosition = static_cast<qint64>(newPosition - 18);
    }
}

void VideoPreview::playNextVideo()
{
    const auto& videoList = timeLine->getVideoList();
    if (currentVideoIndex + 1 < videoList.size())
    {
        currentVideoIndex++;
        updatePlayer();
        currentVideoPosition = mediaPlayer->position();
        mediaPlayer->play();
    }
    else
    {
        currentVideoIndex = 0;
        currentVideoPosition = 0;
        mediaPlayer->setPosition(0);
        updatePlayer();
        emit playbackFinished();
    }
}


void VideoPreview::determineStartEndTimes(const VideoData& videoData, qreal& startTime, qreal& endTime)
{
    startTime = 0;
    endTime = -1;

    startTime = videoData.getStartTime();
    endTime = videoData.getEndTime();
}

void VideoPreview::clearMediaPlayer()
{
    mediaPlayer->stop();
    mediaPlayer->setSource(QUrl());
    videoWidget->update();
}
