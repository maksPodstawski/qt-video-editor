#include "AudioData.h"

AudioData::AudioData(const QString &title, const QString &duration, const QString &filePath, const QString &extension)
: title(title), duration(duration), filePath(filePath), extension(extension) {}

QString AudioData::getTitle() const
{
    return title;
}

QString AudioData::getDuration() const
{
    return duration;
}

QString AudioData::getExtension() const
{
    return extension;
}

QString AudioData::getFilePath() const
{
    return filePath;
}
