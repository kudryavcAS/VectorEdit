#include "customgraphicsview.h"
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QDebug>

#include "rect.h"
#include "circle.h"
#include "straight.h"
#include "point.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setSceneRect(0, 0, viewport()->width(), viewport()->height());
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if (scene())
        scene()->setSceneRect(this->rect()); // Сцена обновляется по размеру view
}

void CustomGraphicsView::setDrawingEnabled(bool enabled)
{
    drawing = enabled;
    setCursor(enabled ? Qt::CrossCursor : Qt::ArrowCursor);
}

void CustomGraphicsView::setShapeType(ShapeType shape)
{
    currentShape = shape;
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (drawing && event->button() == Qt::LeftButton && currentShape != ShapeType::None)
    {
        startPoint = mapToScene(event->pos());

        switch (currentShape) {
        case ShapeType::Rectangle:
            currentItem = new Rect(QRectF(startPoint, QSizeF(0, 0)));
            break;
        case ShapeType::Circle:
            currentItem = new Circle(QRectF(startPoint, QSizeF(0, 0)));
            break;
        case ShapeType::Straight:
            currentItem = new Straight(QLineF(startPoint, startPoint));
            break;
        case ShapeType::Point:
            currentItem = new Point(startPoint);
            scene()->addItem(currentItem); // сразу добавляем точку
            drawing = false;
            setCursor(Qt::ArrowCursor);
            return;
        default:
            break;
        }

        if (currentItem)
            scene()->addItem(currentItem);
    }

    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (!drawing || !currentItem)
    {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }

    QPointF endPoint = mapToScene(event->pos());

    switch (currentShape) {
    case ShapeType::Rectangle:
        if (auto rectItem = dynamic_cast<Rect*>(currentItem))
            rectItem->setRect(QRectF(startPoint, endPoint).normalized());
        break;

    case ShapeType::Circle:
        if (auto circleItem = dynamic_cast<Circle*>(currentItem))
            circleItem->setRect(QRectF(startPoint, endPoint).normalized());
        break;

    case ShapeType::Straight:
        if (auto lineItem = dynamic_cast<Straight*>(currentItem))
            lineItem->setLine(QLineF(startPoint, endPoint));
        break;

    default:
        break;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!drawing || event->button() != Qt::LeftButton)
    {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }

    drawing = false;
    setCursor(Qt::ArrowCursor);
    currentItem = nullptr;

    QGraphicsView::mouseReleaseEvent(event);
}

