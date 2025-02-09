#include "../include/ProjectLoader.h"

ProjectLoader::ProjectLoader(const QString& filePath)
    : filePath(filePath) {}

bool ProjectLoader::loadProject(QList<VideoData>& timelineVideos, QList<QString>& videoLibrary) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject projectObject = doc.object();


    QJsonArray timelineArray = projectObject["timelineVideos"].toArray();
    for (const QJsonValue& value : timelineArray) {
        timelineVideos.append(deserializeVideo(value.toObject()));
    }


    QJsonArray libraryArray = projectObject["videoLibrary"].toArray();
    for (const QJsonValue& value : libraryArray) {
        QJsonObject filmObject = value.toObject();
        QString filmPath = filmObject["path"].toString();
        videoLibrary.append(filmPath);
    }

    file.close();
    return true;
}

VideoData ProjectLoader::deserializeVideo(const QJsonObject& jsonObject) const
{
    VideoData video;
    video.setTitle(jsonObject["title"].toString());
    video.setFilePath(jsonObject["filePath"].toString());
    video.setDuration(jsonObject["duration"].toString());
    QRect rect(jsonObject["x"].toInt(),
               jsonObject["y"].toInt(),
               jsonObject["width"].toInt(),
               jsonObject["height"].toInt());

    video.setColor(QColor(jsonObject["color"].toString()));
    video.setStartTime(jsonObject["startTime"].toDouble());
    video.setEndTime(jsonObject["endTime"].toDouble());
    video.setRect(rect);
    return video;
}