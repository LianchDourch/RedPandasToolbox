#include "graphicsscrollarea.h"

GraphicsScrollArea::GraphicsScrollArea(QSizeF size, QGraphicsItem *parent)
    : QGraphicsObject{parent}, size(size) {
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setFlag(QGraphicsItem::ItemIsFocusable);
}
