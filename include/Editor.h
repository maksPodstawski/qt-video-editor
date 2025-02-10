#ifndef VIDEO_EDITOR_EDITOR_H
#define VIDEO_EDITOR_EDITOR_H

#include "VideoData.h"
#include <QProcess>
#include <QDir>

class Editor
{
public:
    static bool trimVideo(const QString& inputFile, const QString& outputFile, double startTime);
    static bool cutVideo(const QString& inputFile, const QString& outputFile, double cutTime);
    static bool normalizeVideo(const QString& inputFile, const QString& outputFile, const QString& resolution,
                               int frameRate, const QString& videoCodec, const QString& audioCodec, int audioRate,
                               int audioChannels);
    static bool combineVideos(const QList<VideoData>& inputVideos, const QString& outputFile, const QString& resolution,
                              int frameRate);
};

#endif //VIDEO_EDITOR_EDITOR_H
