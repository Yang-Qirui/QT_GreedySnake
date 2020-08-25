#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <ctime>
#include <QPushButton>
#include <QFont>
#include <QLineEdit>

#define MAX_X 40
#define MAX_Y 40
#define Label_Size 20

enum Labeltype{
    bg_label,
    snake_label,
    food_label,
    border_label,
};

struct Snake{
    QLabel *label;
    int type;
    int x;
    int y;
};

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *event);
public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    void initGame(int);
    void initBorder();
    void initSnake();
    void moveSnake();
    void createFood();
    void startGame();
    void pauseGame();
    void continueGame();
    void gameOver();
    void updateSnake();
    void enableCreate();
signals:
    void resetButton();
private:
    int foodCount=0;
    Ui::GameWidget *ui;
    Snake *map_label[MAX_X][MAX_Y];
    int speed;
    QTimer timer;
    QList<Snake*> snake;
    Snake* head;
    Snake* tail;
    int dX,dY;
    QPushButton *_start;
    QPushButton *_pause;
    QPushButton *_continue;
    QPushButton *_createBorder;
public slots:
    void snakeMoveSlots();
    void startGameSlots();
    void pauseGameSlots();
    void continueGameSlots();
    void enableCreateSlots();

};

#endif // GAMEWIDGET_H
