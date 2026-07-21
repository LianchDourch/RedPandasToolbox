#ifndef WIDGETPLACER_H
#define WIDGETPLACER_H

#include <QPoint>
#include <qwidget.h>
#include "RedPandasToolbox_global.h"

struct REDPANDASTOOLBOX_EXPORT WidgetLocation {
public:
    class LocationType {
    public:
        std::function<QPoint(QWidget* parent, QWidget* target, const WidgetLocation&)> func;

        LocationType(std::function<QPoint(QWidget* parent, QWidget* target, const WidgetLocation&)> func)
            : func(func)
        {

        }
    };

    static const LocationType TOP_LEFT;
    static const LocationType CENTER;
    static const LocationType CENTER_TOP;
    static const LocationType CENTER_BOTTOM;

public:
    double x, y;
    const LocationType& anchor;

    WidgetLocation(double x, double y, const LocationType& anchor) : x(x), y(y), anchor(anchor) {}

    QPoint get(QWidget* wdg) { return get(wdg, wdg->parentWidget()); }
    QPoint get(QWidget* wdg, QWidget* parent) {
        if (parent == nullptr) return QPoint(0, 0);
        return anchor.func(parent, wdg, *this);
    }
};

struct REDPANDASTOOLBOX_EXPORT WidgetSize {
    enum SizeType {
        ABSOLUTE = 0, RELATIVE = 1
    };

    double w, h;
    SizeType type = RELATIVE;

    QSize get(QWidget* wdg) { return get(wdg, wdg->parentWidget()); }
    QSize get(QWidget* wdg, QWidget* parent) {
        switch (type) {
        case ABSOLUTE:
            return QSize(static_cast<int>(w), static_cast<int>(h));
        case RELATIVE:
            return parent == nullptr ? QSize(0, 0) : QSize(parent->width() * w, parent->height() * h);
        default:
            return QSize(0, 0);
        }
    }
};

struct REDPANDASTOOLBOX_EXPORT WidgetGeometry {
    WidgetSize size;
    WidgetLocation location;

    WidgetGeometry() : WidgetGeometry(0, 0, WidgetLocation::TOP_LEFT, 0, 0, WidgetSize::ABSOLUTE) {}
    WidgetGeometry(const WidgetLocation& loc, const WidgetSize& size) : size(size), location(loc) {}
    WidgetGeometry(double x, double y, const WidgetLocation::LocationType& locType, double width, double height, const WidgetSize::SizeType& sizeType)
        : WidgetGeometry(WidgetLocation(x, y, locType), WidgetSize(width, height, sizeType))
    {  }

    void place(QWidget* widget, QWidget* relativeTo) {
        widget->resize(size.get(widget, relativeTo));
        widget->move(location.get(widget, relativeTo));
    }

    void place(QWidget* widget) {
        place(widget, widget->parentWidget());
    }
};

#endif // WIDGETPLACER_H
