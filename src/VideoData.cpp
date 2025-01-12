#include "../include/VideoData.h"

VideoData::VideoData(const QString &title, const QString &duration, const QString &filePath, const QString &extension, const QRect &initialRect)
    : title(title), duration(duration), filePath(filePath), extension(extension), rect(initialRect)
{
    color = generateRandomColor();
}

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

void VideoData::setRect(const QRect &rect) {
    this->rect = rect;
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

QColor VideoData::getColor() const {
    return color;
}

QRect VideoData::getRect() const {
    return rect;
}

QColor VideoData::generateRandomColor() {
    return QColor::fromRgb(rand() % 156 + 100, rand() % 156 + 100, rand() % 156 + 100);
}

void VideoData::addOperation(Operation *operation) {
    operations.append(operation);
}

void VideoData::executeOperations() {
    for (Operation *operation: operations) {
        this->filePath = operation->execute();
    }
}

QList<Operation *> VideoData::getOperations() const {
    return this->operations;
}

