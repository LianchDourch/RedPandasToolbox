#include "saveable.h"
#include <filesystem>
#include <qdebug.h>

void SaveManager::addEntry(QString entryName) {
    if (std::filesystem::exists((basePath + entryName).toStdString())) std::filesystem::remove_all((basePath + entryName).toStdString());
    try {
        std::filesystem::create_directories((basePath + entryName).toStdString());
    } catch (const std::filesystem::filesystem_error& e) {
    }
}

bool SaveManager::addValue(QString entryName, Saveable* value) {
    if (!filenames.contains(entryName)) filenames.insert(entryName, 1);

    SaveMap map = value->writeObject();
    QString filename = basePath + entryName + (entryName.endsWith("/") ? "" : "/") + QString::number(filenames[entryName]) + SAVE_EXTENSION;
    filenames[entryName] += 1;

    return write(filename, map);
}
