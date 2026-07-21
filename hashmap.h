#ifndef HASHMAP_H
#define HASHMAP_H

#include "rlinkedlist.h"

#define RHASHMAP_DEFAULTCAPACITY 10
#define RHASHMAP_DYNAMICCAPACITY(count) count >

namespace rpt {
template<typename K, typename V>
class Entry {
private:
    K key;
    V value;

public:
    Entry(K key, V value) : key(key), value(value) { }

    inline const K& getKey() const { return key; }
    inline const V& getValue() const { return value; }
};

template<typename K, typename V>
class HashMap
{
protected:
    Array<LinkedList<Entry<K, V>>> entries;

    inline LinkedList<Entry<K, V>> entryList(const K& key) {

    }
public:
    HashMap(int bufferLength = RHASHMAP_DEFAULTCAPACITY) : entries({bufferLength}) {}



    inline V get(const K& key) {

    }
};
}

rhashtype rHash(int i);
rhashtype rHash(long i);
rhashtype rHash(short i);
rhashtype rHash(char i);
rhashtype rHash(bool i);
rhashtype rHash(QString i);
rhashtype rHash(int i);
rhashtype rHash(int i);
rhashtype rHash(int i);
rhashtype rHash(int i);

#endif // HASHMAP_H
