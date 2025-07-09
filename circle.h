#ifndef CIRCLE_H
#define CIRCLE_H

#include <QGraphicsEllipseItem>

class Circle : public QGraphicsEllipseItem
{
public:
    explicit Circle(const QRectF& rect, QGraphicsItem* parent = nullptr);

    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);
    void setBorderColor(const QColor& color);
};

#endif // CIRCLE_H
