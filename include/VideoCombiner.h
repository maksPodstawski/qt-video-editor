#ifndef VIDEO_EDITOR_VIDEOCOMBINER_H
#define VIDEO_EDITOR_VIDEOCOMBINER_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QList>
#include "Editor.h"


class VideoCombiner : public QThread
{
    Q_OBJECT

public:
    VideoCombiner(const QList<VideoData>& inputVideos, const QString& outputFileName, const QString& resolution,
                  int frameRate, QObject* parent = nullptr);

signals:
    void combineFinished(bool success);

protected:
    void run() override;

private:
    QList<VideoData> inputFiles;
    QString outputFileName;
    QString resolution;
    int frameRate;
};

#endif //VIDEO_EDITOR_VIDEOCOMBINER_H
