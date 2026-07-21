#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <functional>
#include <qdir.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qregularexpression.h>
#include <qvectornd.h>
#include <type_traits>
#include <utility>
#include <iostream>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QRandomGenerator>
#include <QDate>
#include <QLocale>
#include <QSet>
#include <QQueue>
#include <QMap>
#include <QList>
#include <QMutex>
#include "RedPandasToolbox_global.h"

#ifdef _WIN32
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#ifndef DWMWA_CAPTION_COLOR
#define DWMWA_CAPTION_COLOR 35
#endif

#ifndef DWMWA_TEXT_COLOR
#define DWMWA_TEXT_COLOR 36
#endif

#define COLORED_TOP_BAR(r, g, b) HWND hwnd = (HWND)this->winId(); \
COLORREF color = RGB(r, g, b); \
    DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &color, sizeof(color)); \
    BOOL darkMode = TRUE; \
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));

#else
#define COLORED_TOP_BAR(r, g, b) ;
#endif

class REDPANDASTOOLBOX_EXPORT Console {
public:
    const Console& endl() const {
        std::cout << std::endl;
        return *this;
    }

    const Console& operator<<(bool b) const {
        return (*this << (int) b);
    }

    const Console& operator<<(const std::string &str) const {
        std::cout << str;
        return *this;
    }

    const Console& operator<<(const QString &str) const {
        *this << str.toStdString();
        return *this;
    }

    const Console& operator<<(const char* c) const {
        *this << QString(c);
        return *this;
    }

    const Console& println(const QString& str) const {
        *this << str;
        endl();
        return *this;
    }

    const Console& errorln(const QString& error) const {
        *this << "[ERROR] : ";
        println(error);
        return *this;
    }

    const Console& operator<<(int i) const {
        return (*this << QString::number(i));
    }
};

class REDPANDASTOOLBOX_EXPORT Util
{
public:
    template <typename T>
    struct get_object_type;

    template <template  <typename T> class Container, typename ObjectType>
    struct get_object_type<Container<ObjectType>> {
        using type = ObjectType;
    };

    template <typename T, typename Excep>
    T throwError(Excep e) {
        throw e;
    }
    static int randomGaussianInt(int min, int max);

    template <typename K>
    static QList<QPair<K, int>> order(QMap<K, int> map) {
        QList<QPair<K, int>> res = {};
        res.reserve(map.size());

        for (auto it = map.begin(); it != map.end(); ++it) {
            res.append(qMakePair(it.key(), it.value()));
        }

        std::sort(res.begin(), res.end(), [](const QPair<K, int>& a, const QPair<K, int>& b) {
            return a.second > b.second;
        });

        return res;
    }

    static double random() {
        return QRandomGenerator::global()->generateDouble();
    }

    static int randint(int max) {
        return randint(0, max);
    }

    static int randint(int min, int max) {
        return QRandomGenerator::global()->bounded(min, max+1);
    }

    static void print(const char* str) {
        std::cout << str;
    }

    static void print(double d) {
        std::cout << d;
    }

    static void print(qint64 i) {
        std::cout << i;
    }

    static void print(quint64 i) {
        std::cout << i;
    }

    static void print(const QString &msg) {
        std::cout << msg.toStdString();
    }

    static void print(const QByteArray& array) {
        std::cout << array.toHex(' ').toUpper().toStdString();
    }

    static void print() {

    }

    static void print(QRect rect) {
        std::cout << "QRect(" << rect.x() << ", "
                  << rect.y() << ", " << rect.width()
                  << ", " << rect.height() << ")";
    }

    template<typename T1, typename T2, typename... ArgsTypes>
    static void print(T1&& first, T2&& second, ArgsTypes&&... args) {
        print(std::forward<T1>(first));
        print(std::forward<T2>(second));
        (print(std::forward<ArgsTypes>(args)), ...);
    }

    template<typename T, typename... Args>
    static void println(T&& head, Args&&... tail) {
        print(std::forward<T>(head));
        if constexpr (sizeof...(tail) > 0) {
            print(std::forward<Args>(tail)...);
        }
        std::cout << std::endl;
    }

    template<template <typename T> class Container, typename Type>
    static void printContainer(const Container<Type>& container, const std::function<QString(Type)> toStringer) {
        std::cout << "[";
        bool start = true;
        for (Type item: container) {
            if (start) start = false;
            else std::cout << ", ";
            std::cout << toStringer(item).toStdString();
        }
        std::cout << "]";
    }

    static void print(const std::string& string) {
        std::cout << string;
    }

    static void print(const QSet<int>& container) {
        printContainer<QSet, int>(container, [] (int i) { return QString::number(i); });
    }

    static void printBox(QRectF rect) {
        std::cout << "QRectF_Box(" << rect.left() << ", " << rect.top() << ", " << rect.right() << ", " << rect.bottom() << ")";
    }

    static void print(const QList<int>& container) {
        printContainer<QList, int>(container, [] (int i) { return QString::number(i); });
    }

    static void print(QVector2D vec) {
        std::cout << "QVector2D(" << vec.x() << ", " << vec.y() << ")";
    }

    static void print(const QSize& size) {
        std::cout << "QSize(" << size.width() << ", " << size.height() << ")";
    }

    static void print(const QPoint& point) {
        std::cout << "QPoint(" << point.x() << ", " << point.y() << ")";
    }

    static void print(QMap<QString, QString> map) {
        std::cout << "{";
        bool start = true;
        for (const QString &key: map.keys()) {
            if (start) start = false;
            else std::cout << ", ";
            std::cout << key.toStdString() << " = " << map[key].toStdString();
        }
        std::cout << "}";
    }

    static void print(int i) {
        print(QString::number(i));
    }

    static void print(const QPointF& p) {
        std::cout << "QPointF(" << p.x() << ", " << p.y() << ")";
    }

    static void print(const QSizeF& s) {
        std::cout << "QSizeF(" << s.width() << ", " << s.height() << ")";
    }

    static void print(QRectF r) {
        std::cout << "QRectF(" << r.x() << ", " << r.y() << ", " << r.width() << ", " << r.height() << ")";
    }

    template<typename T>
        requires std::same_as<T, bool>
    static void print(T b) {
        std::cout << (b ? "true" : "false");
        if (static_cast<short>(b) > 1) std::cout << "[" << static_cast<short>(b) << "]";
    }

    template<typename T>
    static void println(T t) {
        print(t);
        std::cout << std::endl;
    }

    template<typename T>
    static T printedln(T t) {
        print(t);
        std::cout << std::endl;
        return t;
    }

    static void error(const QString& errorMessage) {
        Util::println("[ERROR] : ", errorMessage);
    }

    inline static Console out = Console();

    static int yearsBetween(const QDate& start, const QDate& end) {
        int years = end.year() - start.year();

        // Si le mois/jour de end est avant celui de start, on enlève 1 an
        if (end.month() < start.month() || (end.month() == start.month() && end.day() < start.day())) {
            years--;
        }

        return years;
    }


    template<typename T, template<typename...> class Container>
    static void addTo(Container<T>& c, const T& value) {
        if constexpr (std::is_same_v<Container<T>, QSet<T>>) {
            c.insert(value);
        } else if constexpr (std::is_same_v<Container<T>, QQueue<T>>) {
            c.enqueue(value);
        } else {
            c.append(value);
        }
    }

    template<typename R, typename T, template<typename...> class Container>
    static R* searchPtr(Container<R*>& list, T (R::* method)() const, const T &test) {
        for (R* r: list) if ((r->*method)() == test) return r;
        return nullptr;
    }

    template<typename R, typename T, template<typename...> class Container>
    static R* searchPtr(Container<R*>& list, T (R::* method)(), const T &test) {
        for (R* r: list) if ((r->*method)() == test) return r;
        return nullptr;
    }

    template<typename T, template<typename...> class ContSource, template<typename...> class ContDest>
    static ContDest<T> convertContainer(const ContSource<T>& cont) {
        ContDest<T> res = ContDest<T>();
        for (const T& t: cont) addTo(res, t);
        return res;
    }

    static QString trivialized(const QString& str) {
        return QString(str).replace(" ", "_").remove(QRegularExpression("[^A-Za-z0-9]"));
    }

    template<typename T, template<typename...> class Container>
    static T first(Container<T>& cont) {
        for (const T& t: cont) return t;
        return T();
    }

    static QString toString(const QDate& date) {
        QLocale french(QLocale::French, QLocale::France);
        return french.toString(date, "dddd d MMMM yyyy");
    }

    static void clearChildren(QWidget* widget) {
        if (!widget || !widget->layout())
            return;

        QLayout* layout = widget->layout();

        // 1. Supprimer les éléments du layout
        while (QLayoutItem* item = layout->takeAt(0)) {
            if (QWidget* childWidget = item->widget()) {
                layout->removeWidget(childWidget);
                childWidget->hide();
                childWidget->deleteLater();
            }
            delete item;
        }

        widget->updateGeometry();
    }

    static void clearWidget(QWidget *w) {
        for (QObject* obj: w->children()) obj->deleteLater();
    }

    static void clearLayout(QLayout* layout) {
        clearChildren(layout->widget());
    }

    static void clearGraphicsItem(QGraphicsItem* item) {
        while (!item->childItems().isEmpty()) {
            delete item->childItems().at(0);
        }
    }

    static void clearGraphicsItemGroup(QGraphicsItemGroup *itemGroup) {
        QList<QGraphicsItem*> items = itemGroup->childItems();

        for (QGraphicsItem* item : items) {
            itemGroup->removeFromGroup(item);
            if (itemGroup->scene() != nullptr) itemGroup->scene()->removeItem(item);
            delete item;
        }
    }


    template<typename Type>
    static std::function<Type*(QWidget*)> QObjectConstructor() {
        return [](QWidget* w) -> Type* { return new Type(w); };
    }

    template<typename T>
    static T random(const QList<T>& list) {
        return list[randint(0, list.length()-1)];
    }

    template<typename T>
    inline static T randomInList(const QList<T>& list) { return random(list); }

    template<typename T, template<typename...> class IterableContainer>
    static T random(IterableContainer<T> container) {
        int index = randint(0, container.size()-1);
        for (const T& t: std::as_const(container)) {
            index -= 1;
            if (index < 0) {
                return t;
            }
        }
        return T();
    }

    template<typename T>
    static QSet<T>& remove(QSet<T>* from, QSet<T> those) {
        for (const T& t: std::as_const(those)) from->remove(t);
        return *from;
    }

    template<typename T, template<typename...> class Container, template<typename...> class Container2 = Container>
    static Container<T>& append(Container<T>& first, Container2<T> second) {
        for (const T& t: std::as_const(second)) addTo(first, t);
        return first;
    }

    static constexpr bool fullyGreaterThan(QSizeF left, QSizeF right) {
        return left.width() > right.width() && left.height() > right.height();
    }

    static constexpr bool fullLessThan(QSizeF left, QSizeF right) {
        return fullyGreaterThan(right, left);
    }

    inline static QStringList listFiles(const QString& path, bool* success = nullptr)
    {
        bool t = false;
        if (success == nullptr) success = &t;
        *success = true;
        QDir dir(path);

        if (!dir.exists()) {
            *success = false;
            return {};
        }

        QFileInfoList files = dir.entryInfoList(
            QDir::Files | QDir::NoDotAndDotDot
            );

        QStringList res;
        res.reserve(files.size());
        for (const QFileInfo& fileInfo : files) {
            res.append(fileInfo.fileName());
        }

        return res;
    }

    static void clearDirectory(const QString& path)
    {
        QDir dir(path);

        if (!dir.exists())
            return;

        const auto entries =
            dir.entryInfoList(
                QDir::NoDotAndDotDot |
                QDir::Files |
                QDir::Dirs);

        for (const auto& entry : entries)
        {
            if (entry.isDir())
            {
                QDir(entry.absoluteFilePath())
                .removeRecursively();
            }
            else
            {
                QFile::remove(
                    entry.absoluteFilePath());
            }
        }
    }
};

template<typename T, template<typename...> class Container>
struct REDPANDASTOOLBOX_EXPORT Concatenator2000 {
    Container<T> container;

    template<template<typename...> class Container2>
    Concatenator2000& append(Container2<T> container2) {
        Util::append(this->container, container2);
        return *this;
    }

    Container<T> build() {
        return container;
    }
};

#endif // UTIL_H
