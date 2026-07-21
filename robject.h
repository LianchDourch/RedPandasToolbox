#ifndef ROBJECT_H
#define ROBJECT_H

#include "RedPandasToolbox_global.h"

#include "rpointers.h"
#include <memory>

#define ROBJECT(typeName, superTypeName) public: using self = typeName; using super = superTypeName;
#define RBASEOBJECT(typeName) ROBJECT(typeName, void)

#define ROBJECTDEF(name) class _##name; using name = rpt::JavaObject<_##name>; class _##name

class REDPANDASTOOLBOX_EXPORT RObjectBase
{
public:
    virtual ~RObjectBase() {}

    template<typename T>
    T* parse() { return dynamic_cast<T*>(this); }
};

class REDPANDASTOOLBOX_EXPORT RSafeObject : public RObjectBase
{
public:

    virtual ~RSafeObject() {
        getReference().onDestroy();
    }

    rpt::SafePtr<RSafeObject> getReference() const { return reference; }
private:
    rpt::SafePtr<RSafeObject> reference = rpt::SafePtr<RSafeObject>(this);
};

class REDPANDASTOOLBOX_EXPORT RJavaObject : public RObjectBase
{
private:
    std::weak_ptr<rpt::ControlBlock> reference;

public:
    virtual ~RJavaObject();

    std::weak_ptr<rpt::ControlBlock> __getControlBlock() { return reference; }

    void __setControlBlock(std::weak_ptr<rpt::ControlBlock> p) { this->reference = p; }
};

using RObject = RObjectBase;

#endif // ROBJECT_H
