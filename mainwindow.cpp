// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customgraphicsview.h"
#include <QColorDialog>
#include <QGraphicsItem>
#include <QFile>
#include <QFileDialog>

#include "rect.h"
#include "circle.h"
#include "straight.h"
#include "freehand.h"
#include "point.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализация сцены один раз
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->setScene(new QGraphicsScene(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_2_triggered()
{
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->setShapeType(CustomGraphicsView::ShapeType::Rectangle);
    customView->setDrawingEnabled(true);
}

void MainWindow::on_action_4_triggered()
{
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->setShapeType(CustomGraphicsView::ShapeType::Circle);
    customView->setDrawingEnabled(true);
}

void MainWindow::on_action_5_triggered()
{
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->setShapeType(CustomGraphicsView::ShapeType::Straight);
    customView->setDrawingEnabled(true);
}

void MainWindow::on_action_6_triggered()
{
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->setShapeType(CustomGraphicsView::ShapeType::Point);
    customView->setDrawingEnabled(true);
}

void MainWindow::on_action_7_triggered()
{
    auto *view = static_cast<CustomGraphicsView*>(ui->graphicsView);
    view->setShapeType(CustomGraphicsView::ShapeType::Freehand);
    view->setDrawingEnabled(true);
}

void MainWindow::on_action_8_triggered()
{
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->undo();
}

void MainWindow::on_action_9_triggered()
{
    QColor color = QColorDialog::getColor(Qt::green, this, "Выберите цвет заливки");
    if (!color.isValid())
        return;

    auto *view = static_cast<CustomGraphicsView*>(ui->graphicsView);
    auto *scene = view->scene();

    if (scene->items().isEmpty())
        return;

    // В Qt последняя добавленная фигура — первая в списке
    for (QGraphicsItem* item : scene->items()) {
        if (auto shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
            // Freehand — не наследует QAbstractGraphicsShapeItem, так что тут мы автоматически его пропускаем
            shape->setBrush(QBrush(color));
            break; // нашли первую подходящую — залили и вышли
        }
    }

}

void MainWindow::on_action_10_triggered()
{
    auto *view = static_cast<CustomGraphicsView*>(ui->graphicsView);
    auto *scene = view->scene();

    if (scene->items().isEmpty())
        return;

    // Пробегаем по всем, ищем первую залитую (последнюю добавленную с заливкой)
    for (QGraphicsItem* item : scene->items()) {
        if (auto shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
            if (shape->brush().style() != Qt::NoBrush) {
                shape->setBrush(Qt::NoBrush);
                break;
            }
        }
    }}

void MainWindow::saveToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Не удалось открыть файл для записи");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0); // или другую подходящую версию

    auto *scene = static_cast<CustomGraphicsView*>(ui->graphicsView)->scene();
    QList<QGraphicsItem*> items = scene->items();
    out << static_cast<qint32>(int(items.size()));  // Кол-во элементов

    for (QGraphicsItem *item : scene->items()) {

        if (auto rect = dynamic_cast<Rect*>(item)) {
            out << static_cast<qint32>(ShapeId::Rect);
            rect->serialize(out);

        } else if (auto circle = dynamic_cast<Circle*>(item)) {
            out << static_cast<qint32>(ShapeId::Circle);
            circle->serialize(out);

        } else if (auto point = dynamic_cast<Point*>(item)) {
            out << static_cast<qint32>(ShapeId::Point);
            point->serialize(out);

        } else if (auto straight = dynamic_cast<Straight*>(item)) {
            out << static_cast<qint32>(ShapeId::Straight);
            straight->serialize(out);

        } else if (auto freehand = dynamic_cast<Freehand*>(item)) {
            out << static_cast<qint32>(ShapeId::Freehand);
            QPainterPath path = freehand->path();
            out << path << freehand->pen(); // простейшая сериализация freehand
        }
    }
}

void MainWindow::on_action_3_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить как", "", "Файлы сцены (*.bin *.dat *.scene)");
    if (!filePath.isEmpty()) {
        saveToFile(filePath);
    }
    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".bin", Qt::CaseInsensitive))
        filePath += ".bin";

}

void MainWindow::on_action_11_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Открыть сцену",
        "",
        "Scene Files (*.bin *.dat *.scene);;Все файлы (*)"
        );

    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);  // Убедись, что соответствует сохранению

    QGraphicsScene* scene = ui->graphicsView->scene();
    scene->clear(); // Очистим перед загрузкой

    qint32 itemCount;
    in >> itemCount;

    for (int i = 0; i < itemCount; ++i) {
        qint32 shapeIdInt;
        in >> shapeIdInt;
        ShapeId shapeId = static_cast<ShapeId>(shapeIdInt);

        QGraphicsItem* item = nullptr;

        switch (shapeId) {
        case ShapeId::Rect: {
            auto* rect = new Rect(QRectF());
            rect->deserialize(in);
            item = rect;
            break;
        }
        case ShapeId::Circle: {
            auto* circle = new Circle(QRectF());
            circle->deserialize(in);
            item = circle;
            break;
        }
        case ShapeId::Point: {
            auto* point = new Point(QPointF());
            point->deserialize(in);
            item = point;
            break;
        }
        case ShapeId::Straight: {
            auto* line = new Straight(QLineF());
            line->deserialize(in);
            item = line;
            break;
        }
        case ShapeId::Freehand: {
            auto* freehand = new Freehand();
            // Реализуй метод deserialize(), если его нет
            // freehand->deserialize(in);
            item = freehand;
            break;
        }
        default:
            qWarning() << "Неизвестная фигура:" << shapeIdInt;
            continue;
        }

        if (item)
            scene->addItem(item);
    }

    qDebug() << "Загружено фигур:" << scene->items().size();

    for (QGraphicsItem *item : scene->items()) {
        qDebug() << "Тип фигуры:" << typeid(*item).name()
                 << "BoundingRect:" << item->boundingRect();
    }

    file.close();
    scene->update();
    ui->graphicsView->viewport()->update();
    qDebug() << "Сцена загружена из:" << filePath;
}

