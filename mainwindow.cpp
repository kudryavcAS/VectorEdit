// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customgraphicsview.h"
#include <QColorDialog>
#include <QGraphicsItem>

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
    }
}

