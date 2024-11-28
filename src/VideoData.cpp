#include "../include/VideoData.h"

VideoData::VideoData(const QString &title, const QString &duration, const QString &filePath, const QString &extension)
: title(title), duration(duration), filePath(filePath), extension(extension) {}

void VideoData::setFilePath(const QString &filePath) {
    this->filePath = filePath;
}

void VideoData::setTitle(const QString &title) {
    this->title = title;
}

void VideoData::setDuration(const QString &duration) {
    this->duration = duration;
}

void VideoData::setExtension(const QString &extension) {
    this->extension = extension;
}

QString VideoData::getTitle() const {
    return title;
}

QString VideoData::getDuration() const {
    return duration;
}

QString VideoData::getExtension() const {
    return extension;
}

QString VideoData::getFilePath() const {
    return filePath;
}

