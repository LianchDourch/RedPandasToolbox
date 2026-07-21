#include "widgetplacer.h"

const WidgetLocation::LocationType WidgetLocation::TOP_LEFT = WidgetLocation::LocationType(
    [](QWidget* parent, QWidget* target, const WidgetLocation& loc) { return QPoint(parent->width() * loc.x, parent->height() * loc.y); }
);
const WidgetLocation::LocationType WidgetLocation::CENTER = WidgetLocation::LocationType(
    [](QWidget* parent, QWidget* target, const WidgetLocation& loc) { return QPoint(parent->width() * loc.x - target->width()/2, parent->height() * loc.y - target->height()/2); }
);
const WidgetLocation::LocationType WidgetLocation::CENTER_BOTTOM = WidgetLocation::LocationType(
    [](QWidget* parent, QWidget* target, const WidgetLocation& loc) { return QPoint(parent->width() * loc.x - target->width()/2, parent->height() * loc.y - target->height()); }
);
const WidgetLocation::LocationType WidgetLocation::CENTER_TOP = WidgetLocation::LocationType(
    [](QWidget* parent, QWidget* target, const WidgetLocation& loc) { return QPoint(parent->width() * loc.x - target->width()/2, parent->height() * loc.y); }
    );
