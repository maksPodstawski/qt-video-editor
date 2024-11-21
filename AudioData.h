#ifndef AUDIODATA_H
#define AUDIODATA_H
#include <QString>


class AudioData {
public:
    AudioData(const QString &title, const QString &duration, const QString &filePath, const QString &extension);

    QString getTitle() const;
    QString getDuration() const;
    QString getExtension() const;
    QString getFilePath() const;

private:
    QString title;
    QString duration;
    QString extension;
    QString filePath;
};



#endif //AUDIODATA_H
