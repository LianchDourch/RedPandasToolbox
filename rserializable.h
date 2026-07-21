#ifndef RSERIALIZABLE_H
#define RSERIALIZABLE_H
/**
 * ===============================================================================
 *
 *          INSTRUCTIONS
 *
 * HOW TO MAKE A SERIALIZABLE:
 * 1. extends public RSerializable
 * 2. make a constructor with no args
 * 3. override the writeObject(QDataStream&)
 * 4. override the readObject(QDataStream&)
 * 5. add the SERIALIZABLE(className) macro somewhere
 *
 * You can bypass 2, 3, 4, 5 by using SERIALIZABLE_PASSIVE(className, superclassName), if you don't have any serialization behavior in this class
 *
 *
 * HOW TO SERIALIZE
 * Call RSerizalizer::save(QFile& file, RSerializable* object);
 *
 *
 * HOW TO DESERIALIZE
 * Call RSerializer::load<type>(QFile& file);
 *
 * ===============================================================================
*/

#include <QtNetwork/qsslsocket.h>
#include <qevent.h>
#include <qobject.h>
#include <QIODevice>
#include <QMap>
#include <QFile>
#include <QDir>
#include "RedPandasToolbox_global.h"

#define SERIALIZABLE(className) public: QString getClassName() override { return #className; } private: static className* __instanciate() { return new className(); } inline static const bool __registered = RSerializable::registerClass(#className, []() { return className::__instanciate(); });
#define SERIALIZABLE_ABSTRACT(className, superClass) public: className() : superClass() {} void writeObject(QDataStream& out) override { superClass::writeObject(out); } void readObject(QDataStream& in) override { superClass::readObject(in); } public:
#define SERIALIZABLE_PASSIVE(className, superClass) SERIALIZABLE(className) SERIALIZABLE_ABSTRACT(className, superClass)

class RSerializable;

typedef std::function<RSerializable*()> RPreDeserializer;

class REDPANDASTOOLBOX_EXPORT RSerializer {
public:
    inline static QMap<QString, RPreDeserializer> CLASSES = {};

    static bool registerClass(QString name, RPreDeserializer constr) {
        CLASSES.insert(name, constr);
        return true;
    }

    static RSerializable* instanciate(QDataStream& reader) {
        QByteArray arr;
        QString type;
        reader >> arr;
        type = QString::fromUtf8(arr);

        return instanciate(type, reader);
    }

    static RSerializable* instanciate(const QString &className, QDataStream& stream);

    template<std::derived_from<RSerializable> T>
        requires (!std::same_as<T, RSerializable>)
    inline static T* deserialize(const QByteArray& arr) {
        return dynamic_cast<T*>(deserialize<RSerializable>(arr));
    }

    template<typename RSerializableType = RSerializable>
        requires (std::same_as<RSerializableType, RSerializable>)
    static RSerializableType* deserialize(const QByteArray& arr) {
        if (arr.length() == 0) return nullptr;
        QDataStream reader = QDataStream(arr);
        return instanciate(reader);
    }

    static QByteArray serialize(RSerializable* obj);

    inline static void save(const QString& fileName, RSerializable* obj) {
        QFile work = QFile{fileName};
        return save(work, obj);
    }
    static void save(QFile& file, RSerializable* obj);

    template<template<typename> class Iterable, std::derived_from<RSerializable> T>
    static void saveContainer(const Iterable<T*>& container, QDir dir, uint numberCharCount = 4) {
        int counter = 1;
        dir.makeAbsolute();
        dir.mkpath(".");
        for (T* t: container) {
            QString str = QString::number(counter);
            QString temp = "";
            for (int i = 0; i < (numberCharCount-str.length()); i++) temp += "0";
            save(dir.absoluteFilePath(temp + str), t);
            counter += 1;
        }
    }

    inline static RSerializable* load(const QString& fileName) {
        QFile w = QFile{fileName};
        return load(w);
    }
    static RSerializable* load(QFile& file);

    template<std::derived_from<RSerializable> T>
    inline static T* load(QFile& file) {
        return dynamic_cast<T*>(load(file));
    }

    template<std::derived_from<RSerializable> T>
    inline static T* load(const QString& fileName) {
        return dynamic_cast<T*>(load(fileName));
    }
};

class REDPANDASTOOLBOX_EXPORT RSerializable
{
public:
    /**
     * @brief writeObject Called when we know what is there, meaning we won't write the type.
     *
     * @param out
     */
    virtual void writeObject(QDataStream& out) {}
    QByteArray writeObject() { // Called when we want the whole object to be serialized and portable
        QByteArray res = {};
        QDataStream writer = QDataStream(&res, QIODevice::WriteOnly);

        writer << getClassName().toUtf8();
        writeObject(writer);
        return res;
    }

    inline QByteArray toByteArray() { return writeObject(); }

    virtual void readObject(QDataStream& in) {}
    virtual QString getClassName() = 0;

    inline static bool registerClass(QString name, RPreDeserializer constr) {
        return RSerializer::registerClass(name, constr);
    }

    inline static RSerializable* instanciate(QDataStream& reader) {
        return RSerializer::instanciate(reader);
    }

    inline static RSerializable* instanciate(const QString &className, QDataStream& stream) {
        return RSerializer::instanciate(className, stream);
    }

    inline static RSerializable* deserialize(const QByteArray& arr) {
        return RSerializer::deserialize(arr);
    }
};

#endif // RSERIALIZABLE_H
