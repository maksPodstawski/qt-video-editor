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
      m_iVideoPosition(0),
      pausedPosition(0)  {
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

void VideoPreview::play() {
    const auto &videoList = timeLine->getVideoList();

    if (videoList.isEmpty()) {
        qWarning() << "No videos to play.";
        return;
    }

    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        return;
    }

    if (pausedPosition > 0) {
        mediaPlayer->setPosition(pausedPosition);
        pausedPosition = 0;
    } else {
        currentVideoIndex = timeLine->getCurrentVideoIndexIndicator();
        updatePlayer();

        int indicatorX = timeLine->indicator->x();
        const VideoData &currentVideo = videoList[currentVideoIndex];
        int startX = currentVideo.getRect().left();
        double timePerUnit = timeLine->getTimePerUnit();

        qreal startTime, endTime;
        determineStartEndTimes(currentVideo, startTime, endTime);

        qint64 newPosition = (indicatorX - startX) * timePerUnit + startTime * 1000;
        mediaPlayer->setPosition(newPosition);
    }

    mediaPlayer->play();
}

void VideoPreview::playVideo(const QString &filePath) {
    mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
    mediaPlayer->play();

}

void VideoPreview::stop() {
    mediaPlayer->stop();
}

void VideoPreview::pause() {
    pausedPosition = mediaPlayer->position();
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

QMediaPlayer* VideoPreview::getMediaPlayer() const {
    return mediaPlayer;
}

void VideoPreview::setVolume(float volume) {
    audioOutput->setVolume(volume);
}

float VideoPreview::getVolume() const {
    return audioOutput->volume();
}

void VideoPreview::updatePlayer() {
    const auto &videoList = timeLine->getVideoList();
    if (currentVideoIndex >= videoList.size()) {
        qWarning() << "Invalid video index:" << currentVideoIndex;
        return;
    }

    const VideoData &currentVideo = videoList[currentVideoIndex];
    mediaPlayer->setSource(QUrl::fromLocalFile(currentVideo.getFilePath()));

    qreal startTime, endTime;
    determineStartEndTimes(currentVideo, startTime, endTime);
    mediaPlayer->setPosition(startTime * 1000);

    disconnect(mediaPlayer, &QMediaPlayer::positionChanged, nullptr, nullptr);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
                updateIndicatorPosition(position);
            });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this, endTime](qint64 position) {
        qint64 endTimeMs = endTime * 1000;
        qDebug() << "END TIME:" << endTime;
        if (endTime != -1 && position >= endTimeMs) {
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
    }
}

void VideoPreview::determineStartEndTimes(const VideoData &videoData, qreal &startTime, qreal &endTime) {
    startTime = 0;
    endTime = -1;

    for (const auto &operation : videoData.getOperations()) {
        if (auto cutLeftOp = dynamic_cast<CutLeftOperation*>(operation)) {
            startTime = cutLeftOp->getOperationTime();
        } else if (auto cutRightOp = dynamic_cast<CutRightOperation*>(operation)) {
            endTime = cutRightOp->getOperationTime();
        }
    }

    qDebug() << "Determining start and end times for video:" << videoData.getTitle() << "Start:" << startTime << "End:" << endTime;
}
