#ifndef QUEUE_H
#define QUEUE_H

#include "RedPandasToolbox_global.h"
#include <functional>
#include "util.h"
#include "rlinkedlist.h"

namespace rpt {
template<typename T>
class REDPANDASTOOLBOX_EXPORT Queue : public rpt::LinkedList<T>
{
public:
    using rpt::LinkedList<T>::LinkedList;

    inline void enqueue(const T& o) {
        this->add(o);
    }

    inline T dequeue(T defaultValue = T()) {
        return this->removeFirst(defaultValue);
    }

    inline T peek(T defaultValue = T()) {
        return first(defaultValue);
    }

    inline void printStructureToConsole(std::function<QString(T)> toStringer) const {
        if (this->isEmpty()) Util::println("Queue()");
        QString res = "Queue(";

        _LinkedItem<T>* current = this->m_first;

        do {
            res += "| " + toStringer(current->getValue()) + " | ";
            if (current->hasNext()) res += "-> ";
        } while (current->hasNext());

        Util::println(res + ")");
    }
};
}

#endif // QUEUE_H
