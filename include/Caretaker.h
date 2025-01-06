#ifndef CARETAKER_H
#define CARETAKER_H

#include "Memento.h"
#include <vector>

class Caretaker
{
public:

    void clearMementosAfter(int index);
    void addMemento(const Memento &memento);
    Memento getMemento(int index) const;

private:
    std::vector<Memento> mementos;
};

#endif // CARETAKER_H