#include "freehand.h"
#include <QPen>

Freehand::Freehand(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setPen(QPen(Qt::red, 2));
}

void Freehand::addPoint(const QPointF &point)
{
    QPainterPath path = this->path();
    if (path.isEmpty()) {
        path.moveTo(point);
    } else {
        path.lineTo(point);
    }
    setPath(path);
}
