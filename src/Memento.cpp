#include "../include/Memento.h"

Memento::Memento(const QList<VideoData>& state) : state(state)
{
}

QList<VideoData> Memento::getState() const { return state; }
