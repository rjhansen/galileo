#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDoubleSpinBox>
#include <memory>
#include <algorithm>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    void updateSightLines(double);
    void degRadChanged(int new_index);
    void kmMiChanged(int new_index);
};

#endif // MAINWINDOW_H
