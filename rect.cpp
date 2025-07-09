#include "rect.h"
#include <QPen>
#include <QBrush>

Rect::Rect(const QRectF& rect, QGraphicsItem* parent)
    : QGraphicsRectItem(rect, parent)
{
    setPen(QPen(Qt::red, 2));
    setBrush(Qt::NoBrush);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void Rect::serialize(QDataStream& stream) const
{
    stream << rect() << pen() << brush();
}

void Rect::deserialize(QDataStream& stream)
{
    QRectF rect;
    QPen pen;
    QBrush brush;
    stream >> rect >> pen >> brush;
    setRect(rect);
    setPen(pen);
    setBrush(brush);
}

void Rect::setBorderColor(const QColor& color) {
    QPen p = this->pen();
    p.setColor(color);
    this->setPen(p);
}
