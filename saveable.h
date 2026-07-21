#ifndef SAVEABLE_H
#define SAVEABLE_H

#include <filesystem>
#include <functional>
#include <qcontainerfwd.h>
#include <QMap>
#include <fstream>
#include <iostream>
#include <qdebug.h>
#include "util.h"

/**
 * HOW TO MAKE A SAVEABLE CLASS
 *
 * 1. extends public Saveable
 *
 * 2. Have a constructor MyClass(SaveMap& map) that set the different attributes from the savemap
 *
 * 3. Have a SaveMap& writeObject(SaveMap& map) override that put the attributes into the map.
 * !! the first line should be map = super::writeObject(map) (super being the superclass or Saveable if you want an empty map)
 *
 * 4. Add the SET_CLASS(className) in your class, or SET_CLASS_PASSIVE(className, superClassName) if you don't do anything in the constructor (like you don't have inner attributes) -> remove the constructor then
 */

#define KEYVAL_SEP ":"
#define ENTRY_SEP "\n"
#define SAVE_EXTENSION ".save"

class Saveable;

typedef QMap<QString, QString> SaveMap;
typedef std::function<Saveable*(SaveMap)> SaveableConstructor;

#define SET_CLASS(className) protected: static Saveable* readObject(SaveMap map) { return new className(map); } inline static bool __lineSet = Saveable::setLine(#className, readObject); virtual QString getClassName() override { return #className; } private:
#define SET_CLASS_PASSIVE(className, superClassName) SET_CLASS(className) public: className(SaveMap& map) : superClassName(map) {} private:
#define KEY_CLASSNAME "classname"

#define SAVE_LIST(manager, entry, list) manager.addEntry(entry); for (auto* a: list) manager.addValue(entry, a);

class Saveable
{
public:
    Saveable() {}
    Saveable(SaveMap& map) {}

    SaveMap writeObject() {
        SaveMap res = {};

        return writeObject(res);
    }

    inline static QMap<QString, SaveableConstructor> LINES = {};

    static bool setLine(QString className, SaveableConstructor constr) {
        LINES.insert(className, constr);
        return true;
    }

    template<typename T>
    T cast() {
        return dynamic_cast<T>(this);
    }

    template<typename T>
    bool isInstanceOf() {
        return cast<T>() != nullptr;
    }

    virtual QString getClassName() = 0;

protected:
    virtual SaveMap& writeObject(SaveMap& map) { map.insert(KEY_CLASSNAME, getClassName()); return map; }
};


class SaveManager {
public:
    inline static QString ABSOLUTE_PATH = QString();




    static QStringList read(QString path) {
        std::ifstream file(path.toStdString());

        if (!file) {
            return QStringList();
        } else {
            QStringList res = {};
            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) res.append(line.c_str());
            }

            file.close();
            return res;
        }
    }

    static Saveable* load(QMap<QString, QString> map) {
        QString classname = map[KEY_CLASSNAME];
        return Saveable::LINES[classname](map);
    }

    template<typename T>
    static T load(QString path) {
        //Util::println("Loading " + path);
        QStringList work = read(path);
        SaveMap map = {};
        QStringList temp;
        for (const QString &str: std::as_const(work)) {
            if (!str.contains(KEYVAL_SEP)) continue;
            temp = str.split(KEYVAL_SEP);
            map.insert(temp.first(), temp.last());
        }

        //Util::print("\tMap: ");
        Util::println(map);
        return load(map)->cast<T>();
    }

    /**
     * @brief load
     * @param path le chemin vers le dossier avec les fichiers dedans
     * @return
     */
    template<typename T, template<typename...> class Container>
    static Container<T> loadEntry_static(QString path) {
        Container<T> res = Container<T>();
        if (!std::filesystem::exists(path.toStdString())) return res;
        for (const auto& entry : std::filesystem::directory_iterator(path.toStdString())) {
            Util::addTo(res, load<T>(QString::fromStdString(entry.path().string())));
        }
        return res;
    }

    template<typename T, template<typename...> class Container>
    Container<T> loadEntry(QString entry) {
        QString path = basePath + entry;

        return loadEntry_static<T, Container>(path);
    }

    static bool write(QString path, SaveMap map) {
        QString res = "";

        for (const QString &key: map.keys()) {
            QString line = key + KEYVAL_SEP + map.value(key) + ENTRY_SEP;
            res += line;
        }

        return write(path, res);
    }

    static bool write(QString path, QString text) {
        std::ofstream fichier(path.toStdString());
        if (!fichier) {
            return false;
        }

        fichier << text.toStdString();

        fichier.close();
        return true;
    }

    SaveManager(QString basePath) : basePath(QString(basePath.contains(":") ? "" : ABSOLUTE_PATH) + (basePath.endsWith("/") ? basePath : (basePath + "/"))) {}
    void addEntry(QString name);
    bool addValue(QString entryName, Saveable* value);

    template <typename T>
    bool saveEntry(QString entryName, QSet<T> values) {
        addEntry(entryName);
        bool success = true;
        for (T t: values) success &= addValue(entryName, t);
        return success;
    }

private:
    QString basePath;
    QMap<QString, int> filenames = {};
};


#endif // SAVEABLE_H
