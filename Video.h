//
// Created by czest on 05.11.2024.
//

#ifndef VIDEO_EDITOR_VIDEO_H
#define VIDEO_EDITOR_VIDEO_H


#include <QString>


class Video {
public:
    Video(const QString &title, const QString &duration, const QString &filePath)
    : title(title), duration(duration), filePath(filePath) {};

    QString getTitle() const {
        return title;
    }
    QString getDuration() const {
        return duration;
    }
    QString getFilePath() const {
        return filePath;
    }


private:
    QString title;
    QString duration;
    QString filePath;
};

#endif //VIDEO_EDITOR_VIDEO_H
