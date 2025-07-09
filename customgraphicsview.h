    #ifndef CUSTOMGRAPHICSVIEW_H
    #define CUSTOMGRAPHICSVIEW_H

    #include <QGraphicsView>
    #include <QGraphicsRectItem>
    #include <QGraphicsEllipseItem>
    #include <QGraphicsLineItem>
    #include <QGraphicsPathItem>
    #include <QGraphicsItem>
    #include <QStack>

enum class ShapeId {
    Rect = 0,
    Circle = 1,
    Point = 2,
    Straight = 3,
    Freehand = 4,
};



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
            Freehand,
            Selection
        };

        void setDrawingEnabled(bool enabled);
        void setShapeType(ShapeType shape);
        void undo();
        void setFillColor(const QColor &color);

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        bool drawing = false;
        QPointF startPoint;
        ShapeType currentShape = ShapeType::None;
        QBrush currentBrush;
        QGraphicsItem* currentItem = nullptr;
        QStack<QGraphicsItem*> undoStack;
    };

    #endif // CUSTOMGRAPHICSVIEW_H
