#ifndef VIDEOPERVIEWWIDGET_H
#define VIDEOPERVIEWWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QMimeData>


class VideoPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPreviewWidget(QWidget *parent = nullptr);

    void play();
    void pause();
    void stop();
    void setPosition(qint64 position);
    void setVolume(float volume);
    qint64 getDuration();
    qint64 getPosition();
    void setVideoFiles(const QList<QString> &files);
    float getVolume() const;

private:
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;
    QList<QString> videoFiles;
    int currentVideoIndex;

private slots:
    void playNextVideo();

signals:
};

#endif // VIDEOPERVIEWWIDGET_H
