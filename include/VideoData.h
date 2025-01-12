#ifndef VIDEO_EDITOR_VIDEO_H
#define VIDEO_EDITOR_VIDEO_H

#include <QColor>
#include <QRect>
#include <QString>
#include "Operation.h"
#include "CutRightOperation.h"

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


    QColor generateRandomColor();


private:
    QString title;
    QString duration;
    QString extension;
    QString filePath;
    QRect rect;
    QColor color;
    QList<Operation*> operations;

};

#endif //VIDEO_EDITOR_VIDEO_H
