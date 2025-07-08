#ifndef STRAIGHT_H
#define STRAIGHT_H

#include <QGraphicsLineItem>

class Straight : public QGraphicsLineItem
{
public:
    explicit Straight(const QLineF& line, QGraphicsItem* parent = nullptr);

    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);
};

#endif // STRAIGHT_H
