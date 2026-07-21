#include "imagewidget.h"

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

ImageWidget::ImageWidget(bool keepRatio, QWidget* parent)
    : keepRatio(keepRatio), QFrame(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
}

void ImageWidget::setPixmap(const QPixmap& pixmap)
{
    originalPixmap = pixmap;
    updateScaledPixmap();
    update();
}

void ImageWidget::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);
    updateScaledPixmap();
}

void ImageWidget::updateScaledPixmap()
{
    if (originalPixmap.isNull() || size().isEmpty())
        return;

    scaledPixmap = originalPixmap.scaled(
        size(),
        keepRatio ? Qt::KeepAspectRatio : Qt::IgnoreAspectRatio,     // déformation autorisée
        Qt::SmoothTransformation  // haute qualité
        );
}

void ImageWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);

    if (background.isValid()) {
        painter.fillRect(rect(), getBackground());
    }
    if (!scaledPixmap.isNull())
        painter.drawPixmap(0, 0, scaledPixmap);

    QFrame::paintEvent(e);
}

void ImageWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked(event->position().toPoint());

    QWidget::mousePressEvent(event);
}
