//
// Created by czest on 09.02.2025.
//

#ifndef OPTIONS_H
#define OPTIONS_H
#include <QString>


class Options {
private:
    QString resolution;
    int frameRate;
public:
    Options(): resolution("1280:720"), frameRate(30) {};
    void setResolution(const QString &resolution);
    void setFrameRate(int frameRate);
    QString getResolution() const;
    int getFrameRate() const;
};



#endif //OPTIONS_H
