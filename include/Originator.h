#ifndef ORIGINATOR_H
#define ORIGINATOR_H

#include <QList>
#include "Memento.h"
#include "VideoData.h"

class Originator
{
public:
    void setState(const QList<VideoData>& state);
    void getStateFromMemento(const Memento& memento);

    QList<VideoData> getState() const;

    Memento saveStateToMemento();

private:
    QList<VideoData> state;
};

#endif // ORIGINATOR_H
