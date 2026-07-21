#ifndef RPOINTERS_H
#define RPOINTERS_H

#include "RedPandasToolbox_global.h"
#include <memory>
#include "util.h"

class RSafeObject;
class RJavaObject;

namespace rpt {

struct REDPANDASTOOLBOX_EXPORT ControlBlock
{
    void* ptr = nullptr;
};

template<typename T>
class REDPANDASTOOLBOX_EXPORT SafePtr
{
    template<typename>
    friend class SafePtr;

private:
    const std::shared_ptr<ControlBlock> m_control;

private:
    friend RSafeObject;
    void onDestroy() {
        this->m_control->ptr = nullptr;
    }

public:
    SafePtr(std::nullptr_t t = nullptr) {}

    SafePtr(const SafePtr& other)
        : m_control(other.m_control)
    {
    }

    SafePtr(SafePtr&& other) noexcept
        : m_control(std::move(other.m_control))
    {
    }

    SafePtr& operator=(const SafePtr& other)
    {
        if (this != &other)
        {
            const_cast<std::shared_ptr<ControlBlock>&>(m_control) = other.m_control;
        }

        return *this;
    }

    SafePtr& operator=(SafePtr&& other) noexcept
    {
        if (this != &other)
        {
            const_cast<std::shared_ptr<ControlBlock>&>(m_control) = std::move(other.m_control);
        }

        return *this;
    }

    template<typename U>
        requires std::derived_from<U, T>
    explicit SafePtr(U* ptr)
        : m_control(std::make_shared<ControlBlock>())
    {
        m_control->ptr = ptr;
    }

    template<typename U>
        requires std::derived_from<U, T> || std::derived_from<T, U>
    SafePtr(const SafePtr<U>& other)
        : m_control{other.m_control}
    {
    }

    T* rawPtr() const
    {
        return m_control
                   ? static_cast<T*>(m_control->ptr)
                   : nullptr;
    }

    bool isNull() const {
        return rawPtr() == nullptr;;
    }

    T& operator*() const
    {
        return *rawPtr();
    }

    T* operator->() const
    {
        return rawPtr();
    }

    inline explicit operator bool() const
    {
        return isNull();
    }

    bool operator==(std::nullptr_t n) const { return isNull(); }
    bool operator!=(std::nullptr_t n) const { return !isNull(); }

    bool operator==(const SafePtr&) const = default;

    bool operator<(const SafePtr<T>& p) const {
        return m_control->ptr < p.m_control->ptr;
    }

    void destroy()
    {
        if (isNull()) {
            Util::error("Destroying null pointer.");
            return;
        } else {
            delete static_cast<T*>(m_control->ptr);
        }
        onDestroy();
    }

    std::shared_ptr<ControlBlock> control() const {
        return m_control;
    }

    template<typename R>
        requires std::derived_from<R, T> || std::derived_from<T, R>
    SafePtr<R> rawCast()
    {
        return SafePtr<R>(m_control);
    }

    template<typename R>
        requires std::derived_from<R, T> || std::derived_from<T, R>
    SafePtr<R> cast()
    {
        return instanceof<R>() ? rawCast<R>() : SafePtr<R>(nullptr);
    }

    template<typename Arg>
    inline bool instanceof() {
        return dynamic_cast<Arg*>(rawPtr()) != nullptr;
    }
protected:
    SafePtr(std::shared_ptr<ControlBlock> control) : m_control(control) {

    }
};
}

template<typename T>
size_t qHash(const rpt::SafePtr<T>& ptr, size_t seed = 0) {
    return qHash(&(*ptr.control()), seed);
}

namespace rpt {

/**
 * @brief The Pointer class
 */
template<class T>
class JavaObject
{
    std::shared_ptr<ControlBlock> control;


    void setJavaPointer(T* obj, std::shared_ptr<rpt::ControlBlock> ptr) {
        if (obj == nullptr) return;
        if constexpr (std::derived_from<T, RJavaObject>) {
            obj->__setControlBlock(std::weak_ptr<rpt::ControlBlock>(ptr));
        }
    }
public:
    JavaObject(std::nullptr_t = nullptr) : JavaObject(static_cast<T*>(nullptr)) {
    }

    explicit JavaObject(T* p)
    {
        if (p)
        {
            control = std::make_shared<ControlBlock>();
            control->ptr = p;
            setJavaPointer(p, control);
        }
    }

    void destroy()
    {
        if (control && control->ptr)
        {
            delete static_cast<T*>(control->ptr);
            control->ptr = nullptr;
        }
    }

    bool isNull() const
    {
        return !control || control->ptr == nullptr;
    }

    T* operator->() const
    {
        return static_cast<T*>(control->ptr);
    }

    T* rawPtr() const
    {
        return control
                   ? static_cast<T*>(control->ptr)
                   : nullptr;
    }

    template<typename Arg>
    inline bool instanceof() {
        return dynamic_cast<Arg*>(rawPtr()) != nullptr;
    }
};
}

#endif // RPOINTERS_H
