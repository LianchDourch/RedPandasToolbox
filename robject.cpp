#include "robject.h"
#include "rpointers.h"

RJavaObject::~RJavaObject() {
    std::shared_ptr<rpt::ControlBlock> b = reference.lock();
    b->ptr = nullptr;
}
