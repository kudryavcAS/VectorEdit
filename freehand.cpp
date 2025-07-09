#include "freehand.h"
#include <QPen>

Freehand::Freehand(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    QPen pen(Qt::red);
    pen.setWidth(44);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(Qt::NoBrush);
    setZValue(999);
}

void Freehand::addPoint(const QPointF &point)
{
    qDebug() << "ddd";
    // Сообщаем Qt, что boundingRect() вот‑вот изменится
    prepareGeometryChange();

    // Добавляем к уже имеющемуся пути
    QPainterPath p = path();
    if (p.isEmpty())
        p.moveTo(point);
    else
        p.lineTo(point);
    setPath(p);
    update(boundingRect());
}

// Теперь boundingRect() всегда «знает» про весь путь + толщину пера
QRectF Freehand::boundingRect() const
{
    QRectF r = path().boundingRect();
    qreal w = pen().widthF();
    return r.adjusted(-w, -w, w, w);
}
