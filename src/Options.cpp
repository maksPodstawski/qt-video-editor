#include "../include/Options.h"

void Options::setResolution(const QString &resolution) {
    this->resolution = resolution;
}

void Options::setFrameRate(int frameRate) {
    this->frameRate = frameRate;
}

QString Options::getResolution() const {
    return this->resolution;
}

int Options::getFrameRate() const {
    return this->frameRate;
}
