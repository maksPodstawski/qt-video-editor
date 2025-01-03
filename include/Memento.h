#ifndef MEMENTO_H
#define MEMENTO_H

#include <QList>
#include "VideoData.h"

class Memento
{
public:
    Memento(const QList<VideoData> &state);
    QList<VideoData> getState() const;

private:
    QList<VideoData> state;
};

#endif // MEMENTO_H