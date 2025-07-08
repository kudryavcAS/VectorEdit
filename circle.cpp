#include "Circle.h"
#include <QPen>
#include <QBrush>

Circle::Circle(const QRectF& rect, QGraphicsItem* parent)
    : QGraphicsEllipseItem(rect, parent)
{
    setPen(QPen(Qt::green, 2));
    setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
}

void Circle::serialize(QDataStream& stream) const
{
    stream << rect() << pen() << brush();
}

void Circle::deserialize(QDataStream& stream)
{
    QRectF rect;
    QPen pen;
    QBrush brush;
    stream >> rect >> pen >> brush;
    setRect(rect);
    setPen(pen);
    setBrush(brush);
}
