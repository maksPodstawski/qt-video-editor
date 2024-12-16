//
// Created by czest on 03.12.2024.
//

#include "../include/Editor.h"
#include <QProcess>
#include <QDir>


bool Editor::combineVideos(const QStringList &inputFiles, const QString &outputFile) {
    if (inputFiles.isEmpty()) {
        qWarning() << "No input files provided.";
        return false;
    }

    QStringList normalizedFiles;

    for (int i = 0; i < inputFiles.size(); ++i) {
        QString inputFile = inputFiles[i];
        if (!QFile::exists(inputFile)) {
            qWarning() << "File does not exist:" << inputFile;
            return false;
        }

        QString normalizedFile = QDir::temp().absoluteFilePath(QString("normalized_%1.mp4").arg(i));
        QStringList normalizeArgs;
        normalizeArgs << "-i" << inputFile
                      << "-vf" << "scale=1280:720"
                      << "-r" << "30"
                      << "-c:v" << "libx264"
                      << "-preset" << "fast"
                      << "-c:a" << "aac"
                      << "-ar" << "48000"
                      << "-ac" << "2"
                      << "-strict" << "experimental"
                      << normalizedFile;


        QProcess normalizeProcess;
        normalizeProcess.start("ffmpeg", normalizeArgs);
        normalizeProcess.waitForFinished();

        if (normalizeProcess.exitStatus() != QProcess::NormalExit || normalizeProcess.exitCode() != 0) {
            qWarning() << "Failed to normalize video:" << inputFile;
            return false;
        }

        normalizedFiles.append(normalizedFile);
    }

    QString tempFile = QDir::temp().absoluteFilePath("video_list.txt");
    QFile file(tempFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to create temporary file:" << tempFile;
        return false;
    }

    QTextStream out(&file);
    for (const QString &normalizedFile : normalizedFiles) {
        out << "file '" << normalizedFile << "'\n";
    }
    file.close();

    QStringList combineArgs;
    combineArgs << "-f" << "concat"
                << "-safe" << "0"
                << "-i" << tempFile
                << "-c:v" << "libx264"
                << "-preset" << "fast"
                << "-c:a" << "aac"
                << "-r" << "30"
                << "-ar" << "48000"
                << "-ac" << "2"
                << outputFile;


    QProcess combineProcess;
    combineProcess.start("ffmpeg", combineArgs);
    combineProcess.waitForFinished();

    if (combineProcess.exitStatus() != QProcess::NormalExit || combineProcess.exitCode() != 0) {
        qWarning() << "Failed to combine videos into:" << outputFile;
        return false;
    }

    file.remove();
    for (const QString &normalizedFile : normalizedFiles) {
        QFile::remove(normalizedFile);
    }

    qDebug() << "Videos successfully combined into:" << outputFile;
    return true;
}

