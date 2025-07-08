#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsItem>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CustomGraphicsView(QWidget *parent = nullptr);

    enum class ShapeType {
        None,
        Rectangle,
        Circle,
        Straight,
        Point,
        Freehand
    };

    void setDrawingEnabled(bool enabled);
    void setShapeType(ShapeType shape);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    bool drawing = false;
    QPointF startPoint;
    ShapeType currentShape = ShapeType::None;

    QGraphicsItem* currentItem = nullptr;
};

#endif // CUSTOMGRAPHICSVIEW_H
