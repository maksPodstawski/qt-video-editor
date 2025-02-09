#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "VideoData.h"

class ProjectLoader {
public:
    ProjectLoader(const QString& filePath);

    bool loadProject(QList<VideoData>& timelineVideos, QList<QString>& videoLibrary) const;

private:
    VideoData deserializeVideo(const QJsonObject& jsonObject) const;

    QString filePath;
};

#endif //PROJECTLOADER_H
