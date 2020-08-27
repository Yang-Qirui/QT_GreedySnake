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
#include <QLCDNumber>

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
    void initGame();
    void initBorder();
    void initSnake();
    void moveSnake();
    void createFood();
    void startGame(double);
    void pauseGame();
    void continueGame(double);
    void enableCreate();
    void saveGame();
    void loadGame();
    void restartGame();
    void quitGame();
    void gameOver();
signals:
    void gameOverSignal();
    void displayStepSignal(int);
    void displayScoreSignal(int);
private:
    int steps=0;
    int scores=0;
    double speed;
    Ui::GameWidget *ui;
    Snake *map_label[MAX_X][MAX_Y];
    QTimer timer;
    QList<Snake*> snake;
    Snake* head;
    Snake* tail;
    int dX,dY;
    bool canCreat;
    bool clicked[MAX_X][MAX_Y];
    bool pressed;
    bool canMove;
public slots:
    void snakeMoveSlots();
};

#endif // GAMEWIDGET_H
