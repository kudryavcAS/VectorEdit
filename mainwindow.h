#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_action_2_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

    void on_action_3_triggered();

    void on_action_11_triggered();

    void on_action_triggered();

    void on_action_12_triggered();

    void on_action_13_triggered();

    void on_action_14_triggered();

protected:
  //  void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
    //void openFromFile();
    void saveToFile(const QString &filePath);
    QString currentFilePath;
    QGraphicsItem* clipboardItem = nullptr;
};

#endif // MAINWINDOW_H
