#include "../include/Caretaker.h"

Memento Caretaker::getMemento(int index) const { return mementos.at(index); }

void Caretaker::addMemento(const Memento &memento) { mementos.push_back(memento); }

void Caretaker::clearMementosAfter(int index)
{
    if (index < mementos.size() - 1) {
        mementos.erase(mementos.begin() + index + 1, mementos.end());
    }
}
