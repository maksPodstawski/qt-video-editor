#include "../include/CutLeftOperation.h"

#include <QDir>
#include "../include/Editor.h"

CutLeftOperation::CutLeftOperation(QString &filePath, double cutTime): Operation(filePath, cutTime){}

QString CutLeftOperation::execute() {
    QString filePath = this->filePath;
    QString cutVideoPath = QDir::temp().absoluteFilePath(QFileInfo(filePath).completeBaseName() + "_cut." + QFileInfo(filePath).suffix());

    if (QFile::exists(cutVideoPath)) {
        QFile::remove(cutVideoPath);
    }

    Editor::cutVideo(filePath, cutVideoPath, this->operationTime);

    return cutVideoPath;
}

QJsonObject CutLeftOperation::serialize() const
{
    QJsonObject jsonObject;
    jsonObject["type"] = "CutLeft";
    jsonObject["filePath"] = filePath;
    jsonObject["operationTime"] = operationTime;
    return jsonObject;
}
