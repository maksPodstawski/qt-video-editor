#ifndef PROJECTSAVER_H
#define PROJECTSAVER_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "VideoData.h"
#include "VideoTable.h"
#include "Options.h"

class ProjectSaver
{
public:
    ProjectSaver(const QList<VideoData>& videoList, const QList<QString>& loadedFilms, const QString& filePath);
    bool saveProject() const;
    QJsonObject serializeVideo(const VideoData& video) const;
private:
    //QJsonObject serializeVideo(const VideoData& video) const;

    QList<VideoData> videoList;
    QList<QString> loadedFilms;
    QString filePath;
};


#endif //PROJECTSAVER_H
