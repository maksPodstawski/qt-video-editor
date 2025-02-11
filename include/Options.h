#ifndef OPTIONS_H
#define OPTIONS_H
#include <QString>
#include <QRegularExpression>


class Options
{
public:
    Options();

    void setResolution(const QString& resolution);
    void setFrameRate(int frameRate);

    QString getResolution() const;
    int getFrameRate() const;

private:
    QString resolution;
    int frameRate;
};


#endif //OPTIONS_H
