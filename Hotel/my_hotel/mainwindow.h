#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hotel_code.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
//bool is_init = false;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_quit_clicked();

    void on_pushButton_continue_clicked();

    void on_pushButton_to_end_clicked();

private:
    Ui::MainWindow *ui;



public:
    bool is_init = false;
    Experiment exp;


};
#endif // MAINWINDOW_H
