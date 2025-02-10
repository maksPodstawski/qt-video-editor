#include "../include/Options.h"

Options::Options() : resolution("1280:720"), frameRate(30)
{
}

void Options::setResolution(const QString& resolution)
{
    this->resolution = resolution;
}

void Options::setFrameRate(int frameRate)
{
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
