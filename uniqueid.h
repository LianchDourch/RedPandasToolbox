#ifndef UNIQUEID_H
#define UNIQUEID_H

#include <QString>
#include "RedPandasToolbox_global.h"
#include <QMap>


class REDPANDASTOOLBOX_EXPORT UniqueID
{
public:
    using ID_Type = quint64;
private:
    ID_Type id = 0;

    inline static QMap<QString, ID_Type> IDS = {};

    UniqueID(ID_Type id) {
        this->id = id;
    }

protected:
    static void setGreatestId(QString key, ID_Type id) {
        if (!IDS.contains(key)) IDS.insert(key, 0);
        IDS[key] = std::max(id, IDS[key]);
    }

    static ID_Type getGreatestId(QString key) {
        return IDS.value(key, 0);
    }
public:

    UniqueID() {}

    static UniqueID registerUid(QString key, ID_Type uid) {
        setGreatestId(key, uid);
        return UniqueID(uid);
    }

    static UniqueID newUid(QString key) {
        ID_Type nid = getGreatestId(key) + 1;
        setGreatestId(key, nid);
        return UniqueID(nid);
    }

    static UniqueID nullId() { return UniqueID(0); }

    constexpr bool isNull() const { return id == 0; }

    static UniqueID view(ID_Type uid) {
        return UniqueID(uid);
    }

    bool operator==(const UniqueID& other) const {
        return this->id == other.id;
    }

    bool operator==(unsigned long v) const {
        return this->id == v;
    }

    bool operator<(const UniqueID& other) const {
        return this->id < other.id;
    }

    inline ID_Type getValue() const { return id; }
    inline ID_Type getQtValue() const { return getValue(); }
    inline QString toString() const { return QString::number(getValue()); }
};


size_t REDPANDASTOOLBOX_EXPORT qHash(UniqueID id);

#endif // UNIQUEID_H

