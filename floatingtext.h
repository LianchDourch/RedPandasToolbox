#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <QObject>
#include <QProperty>
#include <QGraphicsObject>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <qtimer.h>
#include "util.h"
#include "rfunctions.h"

class REDPANDASTOOLBOX_EXPORT FloatingText : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    QRectF m_rect;
    QFont m_font;
    rpt::Runnable m_then;

    int dur_attack = 1000;
    int dur_release = 500;
    int dur_sustain = 500;

public:
    FloatingText(const QString &text, const QColor &color, const QFont& font, rpt::Runnable then = rpt::IRunnable(), QPointF startPos = QPointF(), int zvalue = 100) {
        m_text = text;
        m_color = color;
        m_font = font;
        m_then = then;

        QFontMetricsF m(font);
        m_rect = {0, 0, m.horizontalAdvance(m_text), m.height()};

        setPos(startPos);
        setZValue(zvalue); // S'affiche au-dessus du reste
    }

    QRectF boundingRect() const override {
        return m_rect;
    }

    void setCenterPos(qreal x, qreal y) {
        setPos(x - boundingRect().width() / 2., y - boundingRect().height() / 2.);
    }

    void setTimers(int attack, int sustain, int release) {
        dur_attack = attack;
        dur_sustain = sustain;
        dur_release = release;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setPen(m_color);
        painter->setFont(m_font);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
    }

    void animate() {
        // 1. Animation de montée
        QPropertyAnimation *moveAnim = new QPropertyAnimation(this, "pos");
        moveAnim->setDuration(dur_attack);
        moveAnim->setStartValue(pos());
        moveAnim->setEndValue(pos() + QPointF(0, -50)); // Monte de 50 pixels
        moveAnim->setEasingCurve(QEasingCurve::OutCubic);

        // 2. Animation d'opacité (disparition progressive à la fin)
        QPropertyAnimation *fadeAnim = new QPropertyAnimation(this, "opacity");
        fadeAnim->setDuration(dur_release);
        fadeAnim->setStartValue(1.0);
        fadeAnim->setEndValue(0.0);

        // 3. Groupe séquentiel pour attendre avant de disparaître
        QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
        group->addPause(dur_sustain); // Reste visible 500ms
        group->addAnimation(fadeAnim);

        // Lancer les deux en même temps
        moveAnim->start(QAbstractAnimation::DeleteWhenStopped);
        group->start(QAbstractAnimation::DeleteWhenStopped);

        // Auto-destruction à la fin
        connect(group, &QAnimationGroup::finished, this, &FloatingText::onAnimationFinished);
    }

private slots:
    void onAnimationFinished() {
        QTimer::singleShot(0, [this] () { m_then->run(); deleteLater();});
    }

private:
    QString m_text;
    QColor m_color;
};

#endif // FLOATINGTEXT_H
