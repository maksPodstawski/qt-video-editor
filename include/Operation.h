//
// Created by czest on 12.01.2025.
//

#ifndef VIDEO_EDITOR_OPERATION_H
#define VIDEO_EDITOR_OPERATION_H

#include <QString>

class Operation {
public:
    virtual QString execute() = 0;
    virtual ~Operation() = default;
    explicit Operation(const QString &filePath, double operationTime);
    double getOperationTime() { return operationTime; }
protected:
    QString filePath;
    double operationTime;
};


#endif //VIDEO_EDITOR_OPERATION_H
