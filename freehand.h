#ifndef FREEHAND_H
#define FREEHAND_H

#include <QGraphicsPathItem>

class Freehand : public QGraphicsPathItem
{
public:
    Freehand(QGraphicsItem *parent = nullptr);

    void addPoint(const QPointF& point);
};

#endif // FREEHAND_H
