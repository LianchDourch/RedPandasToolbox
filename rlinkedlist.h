#ifndef RLINKEDLIST_H
#define RLINKEDLIST_H

#include <functional>
#include "array.h"
#include "types.h"
#include "util.h"

namespace rpt {
template<typename T>
class _LinkedItem {
private:
    T item;
    _LinkedItem<T> *next = nullptr;

public:
    _LinkedItem(const T& item) {
        this->item = item;
    }

    ~_LinkedItem() {
        delete next;
    }

    /**
     * @brief setNext delete the previous next item, then set the new item
     * @param item
     */
    inline void setNext(_LinkedItem* item) {
        delete next;
        overrideNext(item);
    }

    inline bool hasNext() {
        return next != nullptr;
    }

    /**
     * @brief overrideNext set the next item
     * @param item
     */
    inline void overrideNext(_LinkedItem* item) {
        next = item;
    }

    inline T &getValueRef() {
        return item;
    }

    inline const T& getValue() const {
        return item;
    }

    inline _LinkedItem<T>* getNext() const {
        return next;
    }
};

template<typename T>
class LinkedList
{
protected:
    _LinkedItem<T>* m_first = nullptr;
    _LinkedItem<T>* m_last = nullptr;
public:
    LinkedList() {}
    ~LinkedList() { delete m_first; }

    inline void add(const T& o) {
        _LinkedItem<T>* work = new _LinkedItem<T>(o);
        if (m_last != nullptr) {
            m_last->setNext(work);
        }
        if (m_first == nullptr) m_first = work;
        m_last = work;
    }

    /**
     * @brief first
     * @return
     * @warning don't use on temporary object
    */
    inline T& first() {
        if (m_first == nullptr) throw rpt::IndexOutOfBounds();

        return m_first->getValueRef();
    }

    [[nodiscard]] inline const T &get(rsizetype index) const {
        if (isEmpty()) throw IndexOutOfBounds();
        _LinkedItem<T>* current = m_first;
        while (index > 0) {
            if (!current->hasNext()) throw IndexOutOfBounds();
            current = current->getNext();
            index --;
        }
        return current->getValueRef();
    }

    inline bool isEmpty() const {
        return m_first == nullptr;
    }

    inline void printStructureToConsole(std::function<QString(T)> toStringer) const {
        if (m_first == nullptr) Util::println("LinkedList()");
        std::string res = "LinkedList(";

        _LinkedItem<T>* current = this->m_first;

        do {
            res += "| " + toStringer(current->getValue()) + " | ";
            if (current->hasNext()) res += "-> ";
            current = current->getNext();
        } while (current->hasNext());

        Util::println(res + ")");
    }

    inline rsize_t length() {
        if (isEmpty()) return 0;
        rsize_t res = 1;

        _LinkedItem<T>* work = m_first;
        while (work->hasNext()) {
            res++;
            work = work->getNext();
        }

        return res;
    }

    inline void clear() {
        while (!isEmpty()) removeFirst();
    }

    inline bool removeAll(const T& o) {
        _LinkedItem<T>* work = m_first;
        _LinkedItem<T>* previous = nullptr;
        bool res = false;
        while (work != nullptr) {
            if (work->getValue() == o) {
                _LinkedItem<T>* next = work->getNext();
                work->overrideNext(nullptr);
                if (work == m_last) m_last = previous;
                if (previous == nullptr) m_first = work->getNext();
                else previous->overrideNext(work->getNext());
                delete work;
                res = true;
            }

            previous = work;
            work = work->getNext();
        }

        return res;
    }

    inline T remove(int i) {
        _LinkedItem<T>* work = m_first, *previous = nullptr;
        for (int ind = 0; ind < i; ind ++) {
            if (work == nullptr) throw rpt::IndexOutOfBounds();
            previous = work;
            work = work->getNext();
        }

        if (work == nullptr) throw rpt::IndexOutOfBounds();

        if (work == m_last) {
            m_last = previous;
        }
        if (previous == nullptr) m_first = work->getNext();
        else {
            previous->overrideNext(work->getNext());
        }
        work->overrideNext(nullptr);
        T res = work->getValue();
        delete work;
        return res;
    }

    inline T removeFirst(T defaultValue = T()) {
        if (m_first == nullptr) return defaultValue;

        T res = m_first->getValue();
        _LinkedItem<T>* previousFirst = m_first;
        m_first = previousFirst->getNext();
        previousFirst->overrideNext(nullptr);
        delete previousFirst;
        if (m_first == nullptr) m_last = nullptr;
        return res;
    }
};
}

#endif // RLINKEDLIST_H
