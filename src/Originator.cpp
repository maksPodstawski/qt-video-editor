#include "../include/Originator.h"

void Originator::setState(const QList<VideoData> &state)
{
    this->state = state;
}

void Originator::getStateFromMemento(const Memento &memento)
{
    state = memento.getState();
}


QList<VideoData> Originator::getState() const { return state; }
Memento Originator::saveStateToMemento() { return Memento(state); }

