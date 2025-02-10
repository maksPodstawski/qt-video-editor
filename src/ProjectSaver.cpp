#include "../include/ProjectSaver.h"

ProjectSaver::ProjectSaver(const QList<VideoData>& videoList, const QList<QString>& loadedFilms,
                           const QString& filePath)
    : videoList(videoList), loadedFilms(loadedFilms), filePath(filePath)
{
}

bool ProjectSaver::saveProject() const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QJsonArray timelineVideos;
    for (const VideoData& video : videoList)
    {
        timelineVideos.append(serializeVideo(video));
    }

    QJsonArray videoLibrary;
    for (const QString& filmPath : loadedFilms)
    {
        QFileInfo fileInfo(filmPath);
        QJsonObject filmObject;
        filmObject["title"] = fileInfo.baseName();
        filmObject["duration"] = VideoTable::getDurationText(filmPath);
        filmObject["format"] = VideoTable::getFileFormat(filmPath);
        filmObject["path"] = filmPath;
        videoLibrary.append(filmObject);
    }

    QJsonObject projectObject;
    projectObject["timelineVideos"] = timelineVideos;
    projectObject["videoLibrary"] = videoLibrary;

    QJsonDocument doc(projectObject);
    file.write(doc.toJson());
    file.close();

    return true;
}

QJsonObject ProjectSaver::serializeVideo(const VideoData& video) const
{
    QJsonObject videoObject;
    videoObject["title"] = video.getTitle();
    videoObject["filePath"] = video.getFilePath();
    videoObject["duration"] = video.getDuration();
    videoObject["x"] = video.getRect().x();
    videoObject["y"] = video.getRect().y();
    videoObject["width"] = video.getRect().width();
    videoObject["height"] = video.getRect().height();
    videoObject["color"] = video.getColor().name();
    videoObject["startTime"] = video.getStartTime();
    videoObject["endTime"] = video.getEndTime();

    QJsonArray operationsArray;
    for (const Operation* operation : video.getOperations())
    {
        operationsArray.append(operation->serialize());
    }
    videoObject["operations"] = operationsArray;

    return videoObject;
}
