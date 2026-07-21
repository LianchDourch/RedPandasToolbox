#ifndef POINTERLIFEKEEPER_H
#define POINTERLIFEKEEPER_H

#include <qset.h>
#include <QMap>

#define LIFE_KEPT_POINTER(type, key) void deleteLater() { if (!PointerLifeKeeper::contains<type>(key, this)) { delete this; } PointerLifeKeeper::requestDeletion(key, this); }

class PointerLifeKeeper
{
public:
    inline static QMap<QString, PointerLifeKeeper*> KEEPERS = {};

};

struct PointerLifeDatas {
    bool deletionRequested = false;
    int asyncTaskCount = 0;

    void requestDeletion() { this->deletionRequested = true; }
};

template<typename PointerType>
class IPointerLifeKeeper : public PointerLifeKeeper {
private:
    QHash<PointerType*, PointerLifeDatas> pointers = {};

public:
    void registerPointer(PointerType* pointer) {

    }
};

#endif // POINTERLIFEKEEPER_H
