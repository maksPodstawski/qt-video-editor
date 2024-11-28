#ifndef VIDEO_EDITOR_VIDEO_H
#define VIDEO_EDITOR_VIDEO_H

#include <QString>

class VideoData {
public:
    VideoData(const QString &title, const QString &duration, const QString &filePath, const QString &extension);

    QString getTitle() const;
    QString getDuration() const;
    QString getExtension() const;
    QString getFilePath() const;

    void setFilePath(const QString &filePath);
    void setTitle(const QString &title);
    void setDuration(const QString &duration);
    void setExtension(const QString &extension);

private:
    QString title;
    QString duration;
    QString extension;
    QString filePath;
};

#endif //VIDEO_EDITOR_VIDEO_H
