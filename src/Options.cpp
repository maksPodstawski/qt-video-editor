#include "../include/Options.h"

Options::Options() : resolution("1280:720"), frameRate(30)
{
}

void Options::setResolution(const QString& resolution)
{
    QRegularExpression regex("^\\d+:\\d+$");
    QRegularExpressionMatch match = regex.match(resolution);

    if (match.hasMatch()) {
        this->resolution = resolution;
    }
}

void Options::setFrameRate(int frameRate)
{
    if (frameRate < 1 || frameRate > 120)
    {
        return;
    }
    this->frameRate = frameRate;
}

QString Options::getResolution() const
{
    return this->resolution;
}

int Options::getFrameRate() const
{
    return this->frameRate;
}
