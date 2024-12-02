#include "../include/VideoPerview.h"

VideoPreview::VideoPreview(QWidget *parent)
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

void VideoPreview::play()
{
    mediaPlayer->play();
}

void VideoPreview::stop()
{
    mediaPlayer->stop();
}

void VideoPreview::pause()
{
    mediaPlayer->pause();
}

void VideoPreview::setPosition(qint64 position)
{
    mediaPlayer->setPosition(position);

}

void VideoPreview::setVideoFiles(const QList<QString> &files)
{
    videoFiles = files;
    currentVideoIndex = 0;
    if (!videoFiles.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(videoFiles[currentVideoIndex]));
    }
}

qint64 VideoPreview::getDuration()
{
   return mediaPlayer->duration();
}

qint64 VideoPreview::getPosition()
{
    return mediaPlayer->position();
}

void VideoPreview::setVolume(float volume)
{
    audioOutput->setVolume(volume);
}

float VideoPreview::getVolume() const
{
    return audioOutput->volume();
}

void VideoPreview::playNextVideo()
{
    if(currentVideoIndex++ < videoFiles.size())
    {
        mediaPlayer->setSource(QUrl::fromLocalFile(videoFiles[currentVideoIndex]));
        mediaPlayer->play();
    }
    else
    {
        currentVideoIndex = 0;
    }
}
