#include "../include/VideoPerview.h"

#include <QThread>
#include <QTimer>


VideoPreview::VideoPreview(TimeLine *timeLine, QWidget *parent)
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

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    setLayout(layout);

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            playNextVideo();
        }
    });
}

void VideoPreview::updateIndicatorPosition(qint64 position) {
    const auto &videoList = timeLine->getVideoList();
    if (videoList.isEmpty()) {
        qWarning() << "Video list is empty.";
        return;
    }

    const VideoData &currentVideo = videoList[currentVideoIndex];
    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);

    qint64 duration = (endTime == -1) ? mediaPlayer->duration() - startTime
                                      : static_cast<qint64>(endTime - startTime);

    if (duration <= 0) {
        qWarning() << "Invalid video duration.";
        return;
    }

    int startX = currentVideo.getRect().left();
    int endX = currentVideo.getRect().right();
    int totalDistance = endX - startX;

    int currentX = startX + ((position - static_cast<qint64>(startTime)) * totalDistance / duration);
    currentX = qBound(startX, currentX, endX);

    qDebug() << "Updating indicator position to:" << currentX;

    timeLine->indicator->move(currentX, timeLine->indicator->y());
    timeLine->update();
}


void VideoPreview::play()
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PausedState) {
        qDebug() << "Resuming playback from position:" << mediaPlayer->position();
        mediaPlayer->play();
        return;
    }

    const auto &videoList = timeLine->getVideoList();
    if (videoList.isEmpty()) {
        qWarning() << "No videos to play.";
        return;
    }

    currentVideoIndex = timeLine->getCurrentVideoIndexIndicator();
    qDebug() << "Current video index from timeline:" << currentVideoIndex;
    updatePlayer();

    int indicatorX = timeLine->indicator->x();
    const VideoData &currentVideo = videoList[currentVideoIndex];
    int startX = currentVideo.getRect().left();
    double timePerUnit = timeLine->getTimePerUnit();

    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);

    qreal newPosition = (indicatorX - startX) * timePerUnit + startTime * 1000;

    mediaPlayer->setPosition(newPosition);
    mediaPlayer->play();
}

void VideoPreview::stop() {
    mediaPlayer->stop();
}

void VideoPreview::pause() {
    mediaPlayer->pause();
}

void VideoPreview::setPosition(qint64 position) {
    mediaPlayer->setPosition(position);
}

qint64 VideoPreview::getDuration() {
    return mediaPlayer->duration();
}

qint64 VideoPreview::getPosition() {
    return mediaPlayer->position();
}

QMediaPlayer::PlaybackState VideoPreview::getPlaybackState() const {
    return mediaPlayer->playbackState();
}

void VideoPreview::setVolume(float volume) {
    audioOutput->setVolume(volume);
}

float VideoPreview::getVolume() const {
    return audioOutput->volume();
}

void VideoPreview::updatePlayer()
{
    const auto &videoList = timeLine->getVideoList();
    if (currentVideoIndex >= videoList.size()) {
        qWarning() << "Invalid video index:" << currentVideoIndex;
        return;
    }

    const VideoData &currentVideo = videoList[currentVideoIndex];
    mediaPlayer->setSource(QUrl::fromLocalFile(currentVideo.getFilePath()));

    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);
    m_iVideoPosition = static_cast<qint64>(startTime);

    disconnect(mediaPlayer, &QMediaPlayer::positionChanged, nullptr, nullptr);
    disconnect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, nullptr, nullptr);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        updateIndicatorPosition(position);
    });

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                qDebug() << "End of media reached for video index:" << currentVideoIndex;
                playNextVideo();
            }
        });

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::BufferedMedia) {
            qDebug() << "BufferedMedia status reached. Setting position to:" << m_iVideoPosition;
            mediaPlayer->setPosition(m_iVideoPosition);
        }
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this, endTime](qint64 position) {
        qDebug() << "END TIME:" << endTime;
        if (endTime != -1 && position >= static_cast<qint64>(endTime)) {
            qDebug() << "Video ended at:" << position;
            mediaPlayer->stop();
            playNextVideo();
        }
    });
}

void VideoPreview::playNextVideo() {
    const auto &videoList = timeLine->getVideoList();
    if (currentVideoIndex + 1 < videoList.size()) {
        currentVideoIndex++;
        updatePlayer();
        mediaPlayer->play();
    } else {
        currentVideoIndex = 0;
        qDebug() << "Restarting playlist from the beginning.";
        updatePlayer();
    }
}


void VideoPreview::determineStartEndTimes(const VideoData &videoData, qreal &startTime, qreal &endTime) {
    startTime = 0;
    endTime = -1;

    startTime = videoData.getStartTime();
    endTime = videoData.getEndTime();

    qDebug() << "Determining start and end times for video:" << videoData.getTitle() << "Start:" << startTime << "End:"
            << endTime;
}
