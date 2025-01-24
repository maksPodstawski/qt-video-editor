//
// Created by czest on 12.01.2025.
//

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "../include/Editor.h"
#include "../include/CutRightOperation.h"

QString CutRightOperation::execute() {
    qDebug() << "CutRightOperation executed";
    QString filePath = this->filePath;
    QString trimmedFilePath = QDir::temp().absoluteFilePath(QFileInfo(filePath).completeBaseName() + "_trimmed." + QFileInfo(filePath).suffix());

    if (QFile::exists(trimmedFilePath)) {
        QFile::remove(trimmedFilePath);
    }

    Editor::trimVideo(filePath, trimmedFilePath, this->operationTime);

    return trimmedFilePath;
}


