#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>
#include <qframe.h>
#include "RedPandasToolbox_global.h"

class REDPANDASTOOLBOX_EXPORT ImageWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ImageWidget(bool keepRatio = false, QWidget* parent = nullptr);

    void setPixmap(const QPixmap& pixmap);
    const QPixmap& pixmap() const { return originalPixmap; }

    void setBackground(const QColor& color) { this->background = color; }
    QColor getBackground() const { return background; }

    inline void setKeepRatio(bool keepRatio) { this->keepRatio = keepRatio; }
    inline bool doesKeepRatio() const { return this->keepRatio; }
signals:
    void clicked(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateScaledPixmap();

    QPixmap originalPixmap;
    QPixmap scaledPixmap;
    bool keepRatio = false;

    QColor background = QColor();
};


#endif // IMAGEWIDGET_H
