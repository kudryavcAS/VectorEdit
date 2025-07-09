#ifndef POINT_H
#define POINT_H

#include <QGraphicsEllipseItem>

class Point : public QGraphicsEllipseItem
{
public:
    explicit Point(const QPointF& center, qreal radius = 3.0, QGraphicsItem* parent = nullptr);

    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);
    void setBorderColor(const QColor& color);
};

#endif // POINT_H
