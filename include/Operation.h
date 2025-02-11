#ifndef VIDEO_EDITOR_OPERATION_H
#define VIDEO_EDITOR_OPERATION_H

#include <QJsonObject>
#include <QString>

class Operation
{
public:
    virtual QString execute() = 0;
    virtual ~Operation() = default;

    explicit Operation(const QString& filePath, double operationTime);
    double getOperationTime() { return operationTime; }
    virtual QJsonObject serialize() const = 0;

protected:
    QString filePath;
    qreal operationTime;
};


#endif //VIDEO_EDITOR_OPERATION_H
