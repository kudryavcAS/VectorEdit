#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customgraphicsview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установим сцену и тип кастомного view
    auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
    customView->setScene(new QGraphicsScene(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // Назначаем сцену после полного показа окна
    static bool initialized = false;
    if (!initialized) {
        auto *customView = static_cast<CustomGraphicsView*>(ui->graphicsView);
        customView->setScene(new QGraphicsScene(this));
        initialized = true;
    }
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
