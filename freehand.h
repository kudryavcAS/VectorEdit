#ifndef FREEHAND_H
#define FREEHAND_H

#include <QGraphicsPathItem>

class Freehand : public QGraphicsPathItem
{
public:
    explicit Freehand(QGraphicsItem *parent = nullptr);

    void addPoint(const QPointF& point);

    // Переопределяем, чтобы возвращать актуальный прямоугольник
    QRectF boundingRect() const override;
};

#endif // FREEHAND_H
