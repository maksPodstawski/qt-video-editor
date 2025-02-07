//
// Created by czest on 03.12.2024.
//

#include "../include/Editor.h"
#include <QProcess>
#include <QDir>


bool Editor::combineVideos(const QList<VideoData> &inputVideos, const QString &outputFile,
                           const QList<TextData> &textList) {
    if (inputVideos.isEmpty()) {
        qWarning() << "No input videos provided.";
        return false;
    }

    QStringList normalizedFiles;

    for (int i = 0; i < inputVideos.size(); ++i) {
        QString inputFile = inputVideos[i].getFilePath();
        qDebug() << "Normalizing video:" << inputFile;
        QString normalizedFile = QDir::temp().absoluteFilePath(QString("normalized_%1.mp4").arg(i));

        if (!normalizeVideo(inputFile, normalizedFile, "1280:720", 30, "libx264", "aac", 48000, 2)) {
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
    for (const QString &normalizedFile: normalizedFiles) {
        out << "file '" << normalizedFile << "'\n";
    }
    file.close();

    QStringList combineArgs;
    combineArgs << "-f" << "concat"
            << "-safe" << "0"
            << "-i" << tempFile
            << "-vf" << "scale=1280:720"
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
    for (const QString &normalizedFile: normalizedFiles) {
        QFile::remove(normalizedFile);
    }

    qDebug() << "Videos successfully combined into:" << outputFile;

    QString finalOutputFile = outputFile;
    if (!textList.isEmpty()) {
        QString tempOutputFile = QDir::temp().absoluteFilePath("combined_with_text.mp4");
        if (!addTextToVideo(outputFile, tempOutputFile, textList)) {
            return false;
        }
        finalOutputFile = tempOutputFile;
        QFile::remove(outputFile);
        QFile::copy(finalOutputFile, outputFile);
    }

    return true;
}

bool Editor::addTextToVideo(const QString &inputFile, const QString &outputFile, const QList<TextData> &textList) {
    if (!QFile::exists(inputFile)) {
        qWarning() << "File does not exist:" << inputFile;
        return false;
    }

    QStringList filterComplexArgs;
    for (const TextData &textData : textList) {
        QString escapedText = textData.getText()
            .replace("'", "\\'")
            .replace(":", "\\:");

        QString drawText = QString("drawtext=text='%1':fontfile='C\\:\\\\Windows\\\\Fonts\\\\arial.ttf':fontsize=%2:fontcolor=white:x=%3:y=%4")
            .arg(escapedText)
            .arg(textData.getFontSize())
            .arg(textData.getPosX())
            .arg(textData.getPosY());

        filterComplexArgs << drawText;
    }

    QString filterComplex = filterComplexArgs.join(",");
    QStringList addTextArgs;
    addTextArgs << "-i" << inputFile
                << "-vf" << filterComplex
                << "-codec:a" << "copy"
                << "-y" << outputFile;

    QProcess addTextProcess;
    addTextProcess.start("ffmpeg", addTextArgs);

    if (!addTextProcess.waitForFinished(300000)) {
        qWarning() << "FFmpeg process timed out";
        addTextProcess.kill();
        return false;
    }

    QByteArray processError = addTextProcess.readAllStandardError();
    if (addTextProcess.exitStatus() != QProcess::NormalExit || addTextProcess.exitCode() != 0) {
        qWarning() << "Failed to add text to video:" << inputFile;
        qWarning() << "FFmpeg error:" << processError;
        return false;
    }
    qWarning() << "ADDING TEXT:" << outputFile;
    return true;
}

bool Editor::trimVideo(const QString &inputFile, const QString &outputFile, double startTime) {
    if (!QFile::exists(inputFile)) {
        qWarning() << "File does not exist:" << inputFile;
        return false;
    }

    QStringList trimArgs;
    trimArgs << "-i" << inputFile
            << "-t" << QString::number(startTime / 1000.0, 'f', 3)
            << "-c" << "copy"
            << outputFile;

    QProcess trimProcess;
    trimProcess.start("ffmpeg", trimArgs);
    trimProcess.waitForFinished();

    if (trimProcess.exitStatus() != QProcess::NormalExit || trimProcess.exitCode() != 0) {
        qWarning() << "Failed to trim video:" << inputFile;
        return false;
    }

    qDebug() << "Video successfully trimmed into:" << outputFile;
    return true;
}

bool Editor::cutVideo(const QString &inputFile, const QString &outputFile, double cutTime) {
    if (!QFile::exists(inputFile)) {
        qWarning() << "File does not exist:" << inputFile;
        return false;
    }

    QStringList trimArgs;
    trimArgs << "-i" << inputFile
            << "-ss" << QString::number(cutTime / 1000.0, 'f', 3)
            << "-c" << "copy"
            << outputFile;

    QProcess trimProcess;
    trimProcess.start("ffmpeg", trimArgs);
    trimProcess.waitForFinished();

    if (trimProcess.exitStatus() != QProcess::NormalExit || trimProcess.exitCode() != 0) {
        qWarning() << "Failed to cut video:" << inputFile;
        return false;
    }

    qDebug() << "Video successfully cut into:" << outputFile;
    return true;
}

bool Editor::normalizeVideo(const QString &inputFile, const QString &outputFile, const QString &resolution,
                            int frameRate,
                            const QString &videoCodec, const QString &audioCodec, int audioRate, int audioChannels) {
    if (!QFile::exists(inputFile)) {
        qWarning() << "File does not exist:" << inputFile;
        return false;
    }

    QStringList normalizeArgs;
    normalizeArgs << "-i" << inputFile
            << "-vf" << QString("scale=%1,unsharp=5:5:1.0:5:5:0.0").arg(resolution)
            << "-r" << QString::number(frameRate)
            << "-c:v" << "libx265"
            << "-preset" << "medium"
            << "-b:v" << "5M"
            << "-c:a" << audioCodec
            << "-b:a" << "192k"
            << "-ar" << QString::number(audioRate)
            << "-ac" << QString::number(audioChannels)
            << "-strict" << "experimental"
            << outputFile;

    QProcess normalizeProcess;
    normalizeProcess.start("ffmpeg", normalizeArgs);
    normalizeProcess.waitForFinished();

    if (normalizeProcess.exitStatus() != QProcess::NormalExit || normalizeProcess.exitCode() != 0) {
        qWarning() << "Failed to normalize video:" << inputFile;
        return false;
    }

    return true;
}
