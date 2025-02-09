#ifndef VIDEO_EDITOR_VIDEO_H
#define VIDEO_EDITOR_VIDEO_H

#include <QColor>
#include <QRect>
#include <QString>
#include "Operation.h"


class Operation;

class VideoData {
public:
    VideoData(const QString &title, const QString &duration, const QString &filePath, const QString &extension, const QRect &initialRect);
    VideoData() : title(""), duration(""), filePath(""), extension(""), rect(QRect()), color(generateRandomColor()) {}

    QString getTitle() const;
    QString getDuration() const;
    QString getExtension() const;
    QString getFilePath() const;
    QColor getColor() const;
    QRect getRect() const;
    void addOperation(Operation* operation);
    void executeOperations();
    QList<Operation*> getOperations() const;


    void setFilePath(const QString &filePath);
    void setTitle(const QString &title);
    void setDuration(const QString &duration);
    void setExtension(const QString &extension);
    void setRect(const QRect &rect);
    void setColor(const QColor& color);

    void setStartTime(qreal startTime);
    void setEndTime(qreal endTime);

    qreal getStartTime() const;
    qreal getEndTime() const;

    QColor generateRandomColor();


private:
    QString title;
    QString duration;
    QString extension;
    QString filePath;
    QRect rect;
    QColor color;
    QList<Operation*> operations;

    qreal startTime;
    qreal endTime;

};

inline qreal VideoData::getStartTime() const {
    return this->startTime;
}

inline qreal VideoData::getEndTime() const {
    return this->endTime;
}
#endif //VIDEO_EDITOR_VIDEO_H
