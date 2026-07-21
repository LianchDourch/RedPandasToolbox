#ifndef ARRAY_H
#define ARRAY_H

#include <exception>
#include <initializer_list>
#include "types.h"
#include "RedPandasToolbox_global.h"

namespace rpt {
class  REDPANDASTOOLBOX_EXPORT IndexOutOfBounds : std::exception {

};

template <typename T>
class ArrayList;

template <typename T>
class REDPANDASTOOLBOX_EXPORT Array
{
private:
    T* m_buffer = nullptr;
    rsize_t m_length = 0;

    friend ArrayList<T>;
public:
    Array() {}
    Array(int size, T defaultValue = T()) : m_buffer(new T[size]), m_length(size) {
        for (int i = 0; i < size; i++) m_buffer[i] = defaultValue;
    }
    Array(std::initializer_list<T> list) : Array(list.size()) {
        int i = 0;
        for (const T& t: list) {
            (*this)[i] = t;
            i++;
        }
    }

    Array(const Array& other) : m_length(other.m_length) {
        if (m_length > 0) {
            m_buffer = new T[m_length];
            for (rsize_t i = 0; i < m_length; ++i)
                m_buffer[i] = other.m_buffer[i];
        } else {
            m_buffer = nullptr;
        }
    }

    Array& operator=(const Array& other) {
        if (this == &other) return *this; // self-assignment check

        // Supprimer l’ancien buffer
        delete[] m_buffer;

        m_length = other.m_length;
        if (m_length > 0) {
            m_buffer = new T[m_length];
            for (rsize_t i = 0; i < m_length; ++i)
                m_buffer[i] = other.m_buffer[i];
        } else {
            m_buffer = nullptr;
        }

        return *this;
    }

    Array(Array&& other) noexcept : m_buffer(other.m_buffer), m_length(other.m_length) {
        other.m_buffer = nullptr;
        other.m_length = 0;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] m_buffer;
            m_buffer = other.m_buffer;
            m_length = other.m_length;
            other.m_buffer = nullptr;
            other.m_length = 0;
        }
        return *this;
    }



    ~Array() {
        delete[] m_buffer;
    }

    constexpr bool isNull() const { return m_buffer == nullptr; }
    constexpr bool isEmpty() const { return m_length == 0; }

    constexpr T& operator[](int index) {
        return index >= length() ? throw IndexOutOfBounds() : m_buffer[index];
    }

    constexpr const T& get(int index) const {
        return m_buffer[index];
    }

    constexpr const T& at(int index) const { return get(index); }

    constexpr rsize_t length() const { return m_length; }
    constexpr rsize_t count() const { return length(); }
    constexpr rsize_t size() const { return length(); }

    Array& assertLength(int length) const {
        if (length < this->length()) {
            throw IndexOutOfBounds();
        }
        return *this;
    }

    bool operator==(const Array<T>& other) const {
        for (int i = 0; i < length(); i++) if (get(i) != other.get(i)) return false;
        return true;
    }

    /**
     * @brief forEach
     * @param func une fonction void(int index, T obj)
     */
    template<typename FunctionType>
    void forEach(const FunctionType& func) {
        for (int i = 0; i < length(); i++) func(i, get(i));
    }

    Array<T> clone() const {
        Array<T> res = {length()};
        for (int i = 0; i < length(); i++) res[i] = get(i);
        return res;
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

    // begin/end pour itérateur mutable
    iterator begin() { return iterator(m_buffer); }
    iterator end() { return iterator(m_buffer + m_length); }

    // begin/end pour const
    const_iterator begin() const { return const_iterator(m_buffer); }
    const_iterator end() const { return const_iterator(m_buffer + m_length); }
    const_iterator cbegin() const { return const_iterator(m_buffer); }
    const_iterator cend() const { return const_iterator(m_buffer + m_length); }
};
}

template<typename X, typename Y>
class REDPANDASTOOLBOX_EXPORT Couple {
private:
    X m_x;
    Y m_y;

public:
    Couple() : Couple(X(), Y()) {}
    Couple(X x, Y y) : m_x(x), m_y(y) {
    }

    constexpr X x() const { return m_x; }
    constexpr Y y() const { return m_y; }
    constexpr X first() const { return m_x; }
    constexpr Y second() const { return m_y; }
};

#endif // ARRAY_H
