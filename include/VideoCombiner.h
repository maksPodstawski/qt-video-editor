//
// Created by czest on 16.12.2024.
//

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
    VideoCombiner(const QList<VideoData> &inputVideos, const QString &outputFileName, const QList<TextData> &textList,QObject *parent = nullptr)
            : QThread(parent), inputFiles(inputVideos), outputFileName(outputFileName), textList(textList) {}

signals:
    void combineFinished(bool success);

protected:
    void run() override {
        bool success = Editor::combineVideos(inputFiles, outputFileName, textList);
        emit combineFinished(success);
    }

private:
    QList<VideoData> inputFiles;
    QList<TextData> textList;
    QString outputFileName;
};


#endif //VIDEO_EDITOR_VIDEOCOMBINER_H
