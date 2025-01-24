//
// Created by czest on 14.01.2025.
//

#include "../include/CutLeftOperation.h"

#include <QDir>
#include "../include/Editor.h"


QString CutLeftOperation::execute() {
    QString filePath = this->filePath;
    QString cutVideoPath = QDir::temp().absoluteFilePath(QFileInfo(filePath).completeBaseName() + "_cut." + QFileInfo(filePath).suffix());

    if (QFile::exists(cutVideoPath)) {
        QFile::remove(cutVideoPath);
    }

    Editor::cutVideo(filePath, cutVideoPath, this->operationTime);

    return cutVideoPath;
}
