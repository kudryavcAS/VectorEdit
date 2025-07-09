#include "Point.h"
#include <QPen>
#include <QBrush>

Point::Point(const QPointF& center, qreal radius, QGraphicsItem* parent)
    : QGraphicsEllipseItem(QRectF(center.x() - radius, center.y() - radius, radius * 2, radius * 2), parent)
{
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::black));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void Point::serialize(QDataStream& stream) const
{
    stream << rect() << pen() << brush();
}

void Point::deserialize(QDataStream& stream)
{
    QRectF rect;
    QPen pen;
    QBrush brush;
    stream >> rect >> pen >> brush;
    setRect(rect);
    setPen(pen);
    setBrush(brush);
}

void Point::setBorderColor(const QColor& color)
{
    QPen p = pen();
    p.setColor(color);
    setPen(p);
}
