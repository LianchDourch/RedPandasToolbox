#ifndef GRAPHICSSCROLLAREA_H
#define GRAPHICSSCROLLAREA_H

#include "util.h"
#include <qgraphicsitem.h>
#include <QGraphicsSceneWheelEvent>
#include <qpainter.h>

// No Scroll Handle actually, TODO ?
class REDPANDASTOOLBOX_EXPORT GraphicsScrollArea : public QGraphicsObject
{
private:
    QGraphicsItem* item = nullptr;
    QSizeF size;
public:
    GraphicsScrollArea(QSizeF size, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override {
        return QRectF{{0, 0}, size};
    }

    qreal getPreferredItemWidth() {
        return viewportSize().width();
    }

    void setContentItem(QGraphicsItem* item) {
        if (this->item != nullptr) {
            this->item->setParentItem(nullptr);
            delete this->item;
        }
        this->item = item;
        if (item != nullptr) {
            item->setParentItem(this);
            item->setPos(0, 0);
        }
    }

    QSizeF viewportSize() {
        return size; // TODO quand tu mettras une scroll handle, il faudra éditer ici
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override {

    }

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override {
        Util::println("Wheel Event ", event->delta());
        qreal delta = event->delta();
        qreal newY = item->y() + delta;
        Util::println("\tnewY ", newY);

        // Calculer les limites pour ne pas scroller dans le vide
        qreal minY = boundingRect().height() - item->boundingRect().height();
        newY = qBound(minY, newY, 0.0); // On bloque entre le haut et le bas
        Util::println("\tfinalY ", newY, " (", minY, ", ", newY, ")");

        item->setY(newY);
        event->accept();
    }
};

#endif // GRAPHICSSCROLLAREA_H
