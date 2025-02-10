#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "../include/Editor.h"
#include "../include/CutRightOperation.h"

CutRightOperation::CutRightOperation(QString &filePath, double trimTime): Operation(filePath, trimTime){}

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

QJsonObject CutRightOperation::serialize() const
{
    QJsonObject jsonObject;
    jsonObject["type"] = "CutRight";
    jsonObject["filePath"] = filePath;
    jsonObject["operationTime"] = operationTime;
    return jsonObject;
}


