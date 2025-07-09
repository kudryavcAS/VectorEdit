#include "customgraphicsview.h"
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QDebug>

#include "rect.h"
#include "circle.h"
#include "straight.h"
#include "freehand.h"
#include "point.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    auto *scene = new QGraphicsScene(this);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setSceneRect(0, 0, viewport()->width(), viewport()->height());
    QBrush currentBrush = Qt::NoBrush;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if (scene())
        scene()->setSceneRect(this->rect());
}

void CustomGraphicsView::setFillColor(const QColor &color)
{
    currentBrush = QBrush(color);
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
            scene()->addItem(currentItem);
            undoStack.push(currentItem);
            return;
        case ShapeType::Freehand:
        //{
            currentItem = new Freehand();
          //scene(  )->addItem(currentItem);
            scene()->addItem(currentItem);
            undoStack.push(currentItem);
            dynamic_cast<Freehand*>(currentItem)->addPoint(startPoint);
            return;// Только один вызов
        //}
        //break;
        default:
            break;
        }

        // Добавляем только если не добавляли раньше
        if (currentItem && currentShape != ShapeType::Freehand && currentShape != ShapeType::Point){
            scene()->addItem(currentItem);
            undoStack.push(currentItem);
        }
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

    case ShapeType::Freehand:
        if (auto freehandItem = dynamic_cast<Freehand*>(currentItem))
            freehandItem->addPoint(endPoint);
        break;

    default:
        break;
    }

    viewport()->update();
    QGraphicsView::mouseMoveEvent(event);
}
void CustomGraphicsView::undo()
{
    if (!undoStack.isEmpty()) {
        QGraphicsItem* lastItem = undoStack.pop();
        scene()->removeItem(lastItem);
        delete lastItem; // чтобы избежать утечки памяти
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!drawing || event->button() != Qt::LeftButton)
    {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }

    currentItem = nullptr;
    QGraphicsView::mouseReleaseEvent(event);
}
