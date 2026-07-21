#ifndef COWPTR_H
#define COWPTR_H

#include <QSharedPointer>

#define COWPTR_COMPATIBLE_BASECLASS public: virtual std::unique_ptr<self> clone() const = 0;
#define COWPTR_COMPATIBLE(baseClassName) public: std::unique_ptr<baseClassName> clone() const override { return std::make_unique<self>(*this);  }

/**
 * @brief The CowPtr class
 * !!! TO USE ME, T has to be a polymorphic type with a
 */
template<typename T>
class CowPtr
{
    QSharedPointer<T> ptr;

public:
    CowPtr() = default;

    explicit CowPtr(T* p)
        : ptr(p)
    {}

    inline void detach()
    {
        if (ptr && ptr.use_count() > 1)
            ptr.reset(ptr->clone().release());
    }

    inline bool isNull() const
    {
        return ptr.isNull();
    }

    inline const T* view() const {
        return ptr.data();
    }

    inline T* peek() {
        return ptr.data();
    }

    inline T* operator->()
    {
        detach();
        return ptr.data();
    }

    inline const T* operator->() const
    {
        return ptr.data();
    }

    inline T& operator*()
    {
        detach();
        return *ptr;
    }

    inline const T& operator*() const
    {
        return *ptr;
    }
};

#endif // COWPTR_H
