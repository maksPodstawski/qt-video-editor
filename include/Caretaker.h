#ifndef CARETAKER_H
#define CARETAKER_H

#include "Memento.h"
#include <vector>

class Caretaker
{
public:
    Memento getMemento(int index) const;

    void clearMementosAfter(int index);
    void addMemento(const Memento& memento);

private:
    std::vector<Memento> mementos;
};
#endif // CARETAKER_H
