#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:

private slots:
    void on__start_triggered();

    void on__pause_triggered();

    void on__save_triggered();

    void on__continue_triggered();

    void on__restart_triggered();

    void on__load_triggered();

    void on__quit_triggered();

    void gameOverSlots();

    void startGameSlots();

    void pauseGameSlots();

    void continueGameSlots();

    void restartGameSlots();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
