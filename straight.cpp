#include "Straight.h"
#include <QPen>

Straight::Straight(const QLineF& line, QGraphicsItem* parent)
    : QGraphicsLineItem(line, parent)
{
    setPen(QPen(Qt::blue, 2));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void Straight::serialize(QDataStream& stream) const
{
    stream << line() << pen();
}

void Straight::deserialize(QDataStream& stream)
{
    QLineF line;
    QPen pen;
    stream >> line >> pen;
    setLine(line);
    setPen(pen);
}
