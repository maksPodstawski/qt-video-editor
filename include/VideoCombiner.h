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
    VideoCombiner(Editor *editor, const QList<QString> &inputFiles, const QString &outputFileName, QObject *parent = nullptr)
            : QThread(parent), editor(editor), inputFiles(inputFiles), outputFileName(outputFileName) {}

signals:
    void combineFinished(bool success);

protected:
    void run() override {
        bool success = editor->combineVideos(inputFiles, outputFileName);
        emit combineFinished(success);
    }

private:
    Editor *editor;
    QList<QString> inputFiles;
    QString outputFileName;
};


#endif //VIDEO_EDITOR_VIDEOCOMBINER_H
