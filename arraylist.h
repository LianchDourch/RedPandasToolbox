#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "array.h"
#include "RedPandasToolbox_global.h"

template<typename T>
class REDPANDASTOOLBOX_EXPORT ArrayList
{
protected:
    rpt::Array<T> buffer = rpt::Array<T>(0);
    rsize itemCount = 0;
public:
    ArrayList() {}
    ArrayList(int size, T defaultValue = T()) : buffer(size, defaultValue), itemCount(size) {}
    ArrayList(std::initializer_list<T> list) : buffer(list), itemCount(list.size()) {}

    constexpr rsize capacity() const {
        return buffer.length();
    }

    constexpr rsize length() const {
        return itemCount;
    }

    constexpr bool isEmpty() const {
        return itemCount == 0;
    }

    rpt::Array<T> toArray() const {
        rpt::Array<T> res = Array<T>(itemCount);
        for (int i = 0; i < itemCount; i++) res[i] = buffer[i];
        return res;
    }

    constexpr rsize size() const { return length(); }

    ArrayList<T>& reserve(rsize newCapacity) {
        if (newCapacity <= capacity()) return *this;
        else {
            rpt::Array<T> work(newCapacity);

            for (int i = 0; i < buffer.length(); i++) work[i] = buffer[i];

            this->buffer = work;
        }
        return *this;
    }

    ArrayList(const ArrayList& other)
        : buffer(other.buffer), itemCount(other.itemCount) {}

    ArrayList& operator=(const ArrayList& other) {
        if (this != &other) {
            buffer = other.buffer;  // Array doit gérer sa propre copie
            itemCount = other.itemCount;
        }
        return *this;
    }

    T* data() { return buffer.m_buffer; }           // mutable
    const T* data() const { return buffer.m_buffer; } // const

    constexpr T& operator[](int i) {
        return i < itemCount ? buffer[i] : throw rpt::IndexOutOfBounds();
    }

    const T& get(int index) const {
        return buffer.get(index);
    }

    void add(const T& item) {
        reserve(itemCount + 1);
        buffer[itemCount] = item;
        itemCount += 1;
    }

    template<template <typename> class Container>
    void addAll(Container<T> container) {
        rsize size = container.size();
        reserve(length() + size);
    }

public:
    class iterator {
        T* ptr;
    public:
        explicit iterator(T* p) : ptr(p) {}
        T& operator*() { return *ptr; }
        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr; return tmp; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
    };

    class const_iterator {
        const T* ptr;
    public:
        explicit const_iterator(const T* p) : ptr(p) {}
        const T& operator*() const { return *ptr; }
        const_iterator& operator++() { ++ptr; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++ptr; return tmp; }
        bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
    };

    // Méthodes begin/end
    iterator begin() { return iterator(data()); }
    iterator end()   { return iterator(data() + itemCount); }

    const_iterator begin()  const { return const_iterator(data()); }
    const_iterator end()    const { return const_iterator(data() + itemCount); }
    const_iterator cbegin() const { return const_iterator(data()); }
    const_iterator cend()   const { return const_iterator(data() + itemCount); }

};

template<typename T>
using List = ArrayList<T>;

#endif // ARRAYLIST_H
