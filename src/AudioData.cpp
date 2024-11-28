#include "../include/AudioData.h"

AudioData::AudioData(const QString &title, const QString &duration, const QString &filePath, const QString &extension)
: title(title), duration(duration), filePath(filePath), extension(extension) {}

void AudioData::setFilePath(const QString &filePath) {
    this->filePath = filePath;
}

void AudioData::setTitle(const QString &title) {
    this->title = title;
}

void AudioData::setDuration(const QString &duration) {
    this->duration = duration;
}

void AudioData::setExtension(const QString &extension) {
    this->extension = extension;
}

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



