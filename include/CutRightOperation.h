#ifndef VIDEO_EDITOR_CUTRIGHTOPERATION_H
#define VIDEO_EDITOR_CUTRIGHTOPERATION_H

#include "Operation.h"
#include "VideoData.h"

class CutRightOperation : public Operation
{
public:
    CutRightOperation(QString& filePath, double trimTime);

    QString execute() override;
    QJsonObject serialize() const override;
};

#endif //VIDEO_EDITOR_CUTRIGHTOPERATION_H
