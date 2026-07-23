#ifndef MASTEREDOBJECT_H
#define MASTEREDOBJECT_H

#include "RedPandasToolbox_global.h"

template<typename T>
class REDPANDASTOOLBOX_EXPORT MasteredObject
{
public:

    void setMaster(T master) { this->master = master; }
    inline T getMaster() const { return this->master; }

private:
    T master = T();
};

#define _MASTERED_OBJECT_METHODS(T) public: inline void setMaster(T master) { this->master = master; } inline T getMaster() { return master; }
#define MASTERED_OBJECT_PTR(T) private: T* master = nullptr; _MASTERED_OBJECT_METHODS(T*)
#define MASTERED_OBJECT_POD(T) private: T master = T(); _MASTERED_OBJECT_METHODS(T)

#endif // MASTEREDOBJECT_H
