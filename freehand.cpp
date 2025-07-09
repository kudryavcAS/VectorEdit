#include "freehand.h"
#include <QPainterPath>
#include <QPen>

Freehand::Freehand(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    QPen pen(Qt::red);
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(Qt::NoBrush);
    setZValue(999);
}

void Freehand::addPoint(const QPointF &point)
{
    prepareGeometryChange();

    QPainterPath p = path();
    if (p.isEmpty())
        p.moveTo(point);
    else
        p.lineTo(point);

    setPath(p);
    update(boundingRect());
}

QRectF Freehand::boundingRect() const
{
    if (path().isEmpty())
        return QRectF();

    QRectF r = path().boundingRect();
    qreal w = pen().widthF();
    return r.adjusted(-w, -w, w, w);
}

void Freehand::serialize(QDataStream &stream) const
{
    stream << path() << pen();
}

void Freehand::deserialize(QDataStream &stream)
{
    QPainterPath loadedPath;
    QPen loadedPen;
    stream >> loadedPath >> loadedPen;
    setPath(loadedPath);
    setPen(loadedPen);
}

