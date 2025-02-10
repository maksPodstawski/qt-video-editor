#include "../include/VideoCombiner.h"

VideoCombiner::VideoCombiner(const QList<VideoData>& inputVideos, const QString& outputFileName,
                             const QString& resolution,
                             int frameRate, QObject* parent)
    : QThread(parent), inputFiles(inputVideos), outputFileName(outputFileName), resolution(resolution),
      frameRate(frameRate)
{
}

void VideoCombiner::run()
{
    bool success = Editor::combineVideos(inputFiles, outputFileName, resolution, frameRate);
    emit combineFinished(success);
}
