// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customgraphicsview.h"
#include <QColorDialog>
#include <QGraphicsItem>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>

#include "rect.h"
#include "circle.h"
#include "straight.h"
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

    QList<QGraphicsItem*> selectedItems = scene->selectedItems();

    for (QGraphicsItem* item : selectedItems) {
        if (auto shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
            shape->setBrush(QBrush(color));
        }
    }

}

void MainWindow::on_action_10_triggered()
{
    auto *view = static_cast<CustomGraphicsView*>(ui->graphicsView);
    auto *scene = view->scene();

    if (scene->selectedItems().isEmpty())
        return;

    for (QGraphicsItem* item : scene->selectedItems()) {
        if (auto shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
            shape->setBrush(Qt::NoBrush);
        }
    }
}

void MainWindow::saveToFile(const QString &filePath)
{
    currentFilePath = filePath;  // <-- сохраняем путь
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Не удалось открыть файл для записи");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);

    auto *scene = static_cast<CustomGraphicsView*>(ui->graphicsView)->scene();
    QList<QGraphicsItem*> items = scene->items();
    out << static_cast<qint32>(items.size());

    for (QGraphicsItem *item : items) {
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
        };
    };
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

void MainWindow::on_action_triggered()
{
    if (currentFilePath.isEmpty()) {
        on_action_3_triggered(); // вызываем "Сохранить как..."
    } else {
        saveToFile(currentFilePath);
    }
}

void MainWindow::on_action_12_triggered()
{
    auto *scene = ui->graphicsView->scene();
    QList<QGraphicsItem*> selected = scene->selectedItems();

    if (selected.isEmpty())
        return;

    QGraphicsItem* item = selected.first();  // копируем только один элемент пока что

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out.setVersion(QDataStream::Qt_6_0);

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
    }

    clipboardItem = new QGraphicsRectItem();  // просто для хранения данных, не используется напрямую
    clipboardItem->setData(0, buffer.data());
}

void MainWindow::on_action_13_triggered()
{
    if (!clipboardItem)
        return;

    QByteArray data = clipboardItem->data(0).toByteArray();
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    in.setVersion(QDataStream::Qt_6_0);

    qint32 shapeIdInt;
    in >> shapeIdInt;
    ShapeId shapeId = static_cast<ShapeId>(shapeIdInt);

    QGraphicsItem* newItem = nullptr;

    switch (shapeId) {
    case ShapeId::Rect: {
        auto* rect = new Rect(QRectF());
        rect->deserialize(in);
        newItem = rect;
        break;
    }
    case ShapeId::Circle: {
        auto* circle = new Circle(QRectF());
        circle->deserialize(in);
        newItem = circle;
        break;
    }
    case ShapeId::Point: {
        auto* point = new Point(QPointF());
        point->deserialize(in);
        newItem = point;
        break;
    }
    case ShapeId::Straight: {
        auto* line = new Straight(QLineF());
        line->deserialize(in);
        newItem = line;
        break;
    }
    default:
        qWarning() << "Неизвестная фигура при вставке";
        return;
    }

    if (newItem) {
        //  Получаем текущую позицию курсора мыши в координатах сцены
        QPointF mouseScenePos = ui->graphicsView->mapToScene(ui->graphicsView->mapFromGlobal(QCursor::pos()));

        //  Центр новой фигуры
        QPointF center = newItem->sceneBoundingRect().center();

        //  Смещаем фигуру так, чтобы она оказалась под мышкой
        QPointF offset = mouseScenePos - center;
        newItem->moveBy(offset.x(), offset.y());

        // Добавляем в сцену
        newItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        ui->graphicsView->scene()->addItem(newItem);
        newItem->setSelected(true);  // сразу выделим
    }
}

void MainWindow::on_action_14_triggered()
{
    auto *view = static_cast<CustomGraphicsView*>(ui->graphicsView);
    view->setShapeType(CustomGraphicsView::ShapeType::Selection);
    view->setDrawingEnabled(false);  // отключаем рисование
}

void MainWindow::changeSelectedItemsBorderColor(const QColor& color) {
    for (QGraphicsItem* item : ui->graphicsView->scene()->selectedItems()) {
        if (auto rect = qgraphicsitem_cast<Rect*>(item)) {
            rect->setBorderColor(color);
        } else if (auto circle = qgraphicsitem_cast<Circle*>(item)) {
            circle->setBorderColor(color);
        } else if (auto line = qgraphicsitem_cast<Straight*>(item)) {
            line->setBorderColor(color);
        } else if (auto point = qgraphicsitem_cast<Point*>(item)) {
            point->setBorderColor(color);  // ⬅️ Теперь это работает
        }
    }
}

void MainWindow::on_action_15_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Выберите цвет границы");
    if (color.isValid()) {
        changeSelectedItemsBorderColor(color);
    }
}

