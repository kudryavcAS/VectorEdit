#ifndef FREEHAND_H
#define FREEHAND_H

#include <QGraphicsPathItem>
#include <QDataStream>

class Freehand : public QGraphicsPathItem
{
public:
    explicit Freehand(QGraphicsItem *parent = nullptr);

    void addPoint(const QPointF& point);
    QRectF boundingRect() const override;

    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);
};

#endif // FREEHAND_H
