#ifndef RECT_H
#define RECT_H

#include <QGraphicsRectItem>

class Rect : public QGraphicsRectItem
{
public:
    explicit Rect(const QRectF& rect, QGraphicsItem* parent = nullptr);

    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);
    void setBorderColor(const QColor& color);
};

#endif // RECT_H
