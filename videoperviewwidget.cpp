#include "videoperviewwidget.h"

VideoPreviewWidget::VideoPreviewWidget(QWidget *parent)
    : QWidget{parent},
    mediaPlayer(new QMediaPlayer(this)),
    videoWidget(new QVideoWidget(this)),
    audioOutput(new QAudioOutput(this))
{
    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setAudioOutput(audioOutput);

    audioOutput->setVolume(0.4);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    setLayout(layout);
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

void VideoPreviewWidget::setVideoFile(const QString &filePath)
{
    mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
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
