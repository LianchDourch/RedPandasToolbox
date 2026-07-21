#include "rserializable.h"

RSerializable* RSerializer::instanciate(const QString &className, QDataStream& stream) {
    RSerializable* res = CLASSES.value(className)();
    res->readObject(stream);
    return res;
}

QByteArray RSerializer::serialize(RSerializable* obj) {
    if (obj == nullptr) return QByteArray();
    return obj->toByteArray();
}

void RSerializer::save(QFile& file, RSerializable* obj) {
    QFileInfo info(file);
    if (info.dir().path() != ".") {
        info.dir().mkpath(".");
    }
    file.open(QIODevice::WriteOnly);
    QByteArray arr = std::move(serialize(obj));
    file.write(arr);
    file.close();
}

RSerializable* RSerializer::load(QFile& file) {
    file.open(QIODevice::ReadOnly);

    QByteArray work = std::move(file.readAll());
    return deserialize(work);
}
