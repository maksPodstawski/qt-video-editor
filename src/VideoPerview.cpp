#include "../include/VideoPerview.h"

VideoPreview::VideoPreview(TimeLine *timeLine, QWidget *parent)
        : QWidget{parent},
          mediaPlayer(new QMediaPlayer(this)),
          videoWidget(new QVideoWidget(this)),
          audioOutput(new QAudioOutput(this)),
          timeLine(timeLine),
          currentVideoIndex(0) {
    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setAudioOutput(audioOutput);

    audioOutput->setVolume(0.4);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    setLayout(layout);

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            playNextVideo();
        }
    });
}

void VideoPreview::play() {
    const auto &videoList = timeLine->getVideoList();
    for (auto &video: videoList) {
        qDebug() << video.getFilePath();
    }
    updatePlayer();
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

void VideoPreview::setVolume(float volume) {
    audioOutput->setVolume(volume);
}

float VideoPreview::getVolume() const {
    return audioOutput->volume();
}

void VideoPreview::updatePlayer() {
    if(currentVideoIndex >= timeLine->getVideoList().size()) {
        return;
    }
    mediaPlayer->setSource(QUrl::fromLocalFile(timeLine->getVideoList()[currentVideoIndex].getFilePath()));
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