//
// Created by czest on 12.01.2025.
//

#ifndef VIDEO_EDITOR_CUTRIGHTOPERATION_H
#define VIDEO_EDITOR_CUTRIGHTOPERATION_H


#include "Operation.h"
#include "VideoData.h"

class CutRightOperation : public Operation {
public:
    CutRightOperation(QString &filePath, double trimTime): Operation(filePath), trimStartTime(trimTime) {};
    QString execute() override;

private:
    double trimStartTime;
};


#endif //VIDEO_EDITOR_CUTRIGHTOPERATION_H
