#include "videoperviewwidget.h"

VideoPreviewWidget::VideoPreviewWidget(QWidget *parent)
    : QWidget{parent},
    mediaPlayer(new QMediaPlayer(this)),
    videoWidget(new QVideoWidget(this)),
    audioOutput(new QAudioOutput(this)),
    currentVideoIndex(0)
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

void VideoPreviewWidget::play()
{
    mediaPlayer->play();
}

void VideoPreviewWidget::stop()
{
    mediaPlayer->stop();
}

void VideoPreviewWidget::pause(){
    mediaPlayer->pause();
}

void VideoPreviewWidget::setPosition(qint64 position)
{
    mediaPlayer->setPosition(position);
}

void VideoPreviewWidget::setVideoFiles(const QList<QString> &files)
{
    videoFiles = files;
    currentVideoIndex = 0;
    if (!videoFiles.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(videoFiles[currentVideoIndex]));
    }
}

qint64 VideoPreviewWidget::getDuration()
{
   return mediaPlayer->duration();
}

qint64 VideoPreviewWidget::getPosition()
{
    return mediaPlayer->position();
}

void VideoPreviewWidget::setVolume(float volume)
{
    audioOutput->setVolume(volume);
}

float VideoPreviewWidget::getVolume() const {
    return audioOutput->volume();
}

void VideoPreviewWidget::playNextVideo()
{
    if(++currentVideoIndex < videoFiles.size()){
        mediaPlayer->setSource(QUrl::fromLocalFile(videoFiles[currentVideoIndex]));
        mediaPlayer->play();
    }
    else{
        currentVideoIndex = 0;
    }
}
