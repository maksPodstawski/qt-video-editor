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
    explicit Operation(const QString &filePath);
protected:
    QString filePath;
};


#endif //VIDEO_EDITOR_OPERATION_H
