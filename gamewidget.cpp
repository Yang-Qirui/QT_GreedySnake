#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QtDebug>
#include <windows.h>
#include <QThread>
#include <QStyle>
#include <QtGlobal>
#include <QTime>
#include <QPushButton>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    this->resize((MAX_X+10)*Label_Size,(MAX_Y+1)*Label_Size);
    _start=new QPushButton("Start",this);
    _start->setGeometry((MAX_X+2)*Label_Size,Label_Size,120,40);
    _start->setFocusPolicy(Qt::NoFocus);
    _pause=new QPushButton("Pause",this);
    _pause->setGeometry((MAX_X+2)*Label_Size,3.5*Label_Size,120,40);
    _pause->setFocusPolicy(Qt::NoFocus);
    _continue=new QPushButton("Continue",this);
    _continue->setGeometry((MAX_X+2)*Label_Size,6*Label_Size,120,40);
    _continue->setFocusPolicy(Qt::NoFocus);
    _continue->setEnabled(false);
    _createBorder=new QPushButton("Create Border",this);
    _createBorder->setGeometry((MAX_X+2)*Label_Size,8.5*Label_Size,120,40);
    _createBorder->setFocusPolicy(Qt::NoFocus);
    initGame(75);
    initBorder();
    initSnake();
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(snakeMoveSlots()));
    QObject::connect(_start,SIGNAL(clicked()),this,SLOT(startGameSlots()));
    QObject::connect(_pause,SIGNAL(clicked()),this,SLOT(pauseGameSlots()));
    QObject::connect(_continue,SIGNAL(clicked()),this,SLOT(continueGameSlots()));
    QObject::connect(_createBorder,SIGNAL(clicked()),this,SLOT(enableCreateSlots));
}

void GameWidget::initGame(int moveSpeed){
    this->setFocusPolicy(Qt::TabFocus);
    foodCount=0;
    speed=moveSpeed;
    for (int x=0;x<MAX_X;x++){
        for (int y=0;y<MAX_Y;y++){
            QLabel *label=new QLabel(this);
            label->setGeometry(x*Label_Size+10,y*Label_Size+10,Label_Size,Label_Size);
            label->setStyleSheet("background:gray");
            label->hide();
            map_label[x][y]=new Snake;
            map_label[x][y]->x=x;
            map_label[x][y]->y=y;
            map_label[x][y]->type=bg_label;
            map_label[x][y]->label=label;
        }
    }
}

void GameWidget::initBorder(){
    for (int y=0;y<MAX_Y;y++){
        map_label[0][y]->type=border_label;
        map_label[0][y]->label->setStyleSheet("background:black");
        map_label[0][y]->label->show();
        map_label[MAX_X-1][y]->type=border_label;
        map_label[MAX_X-1][y]->label->setStyleSheet("background:black");
        map_label[MAX_X-1][y]->label->show();
    }
    for (int x=0;x<MAX_X;x++){
        map_label[x][0]->type=border_label;
        map_label[x][0]->label->setStyleSheet("background:black");
        map_label[x][0]->label->show();
        map_label[x][MAX_Y-1]->type=border_label;
        map_label[x][MAX_Y-1]->label->setStyleSheet("background:black");
        map_label[x][MAX_Y-1]->label->show();
    }
}

void GameWidget::initSnake(){
    int initX=10,initY=10;
    int snakelen=2;
    dX = 1;
    dY = 0;
    snake.clear();
    for (int i=0;i<snakelen;i++){
        map_label[initX+i][initY]->type=snake_label;
        map_label[initX+i][initY]->label->setStyleSheet("background:green");
        map_label[initX+i][initY]->label->show();
        snake.append(map_label[initX+i][initY]);
    }
}

void GameWidget::moveSnake(){
    tail=snake.at(0);
    head=snake.at(snake.length()-1);
    Snake* tmp=map_label[head->x+dX][head->y+dY];
    tmp->label->setStyleSheet("background:green");
    tmp->label->show();
    if (tmp->type==border_label||tmp->type==snake_label){
        gameOver();
    }
    else{
        if (tmp->type==food_label){
            tmp->type=snake_label;
            for (int i=0;i<3;i++) snake.append(tmp);
            createFood();
        }else{
            tmp->type=snake_label;
            snake.append(tmp);
            tail->type=bg_label;
            tail->label->hide();
            snake.removeFirst();
        }
    }
}

void GameWidget::updateSnake(){
    for (int i=snake.length()-1;i>0;i--){
        QLabel* label=snake[i-1]->label;
        snake[i]->label=label;
    }
}

void GameWidget::createFood(){
    int foodX=0;int foodY=0;
    srand((unsigned)time(0));
    do{
        foodX = rand()%MAX_X;
        foodY = rand()%MAX_Y;
    }while(map_label[foodX][foodY]->type != bg_label);
    map_label[foodX][foodY]->type=food_label;
    map_label[foodX][foodY]->label->setStyleSheet("background:red");
    map_label[foodX][foodY]->label->show();
}

void GameWidget::keyPressEvent(QKeyEvent *e) {
    switch(e->key()) {

    case Qt::Key_Left:
        if(dX == 0){
            dX = -1;
            dY = 0;
        }
        break;
    case Qt::Key_Right:
        if(dX == 0){
            dX = 1;
            dY = 0;
        }
        break;
    case Qt::Key_Up:
        if(dY == 0){
            dX = 0;
            dY = -1;
            }
        break;
    case Qt::Key_Down:
        if(dY == 0){
            dX = 0;
            dY = 1;
            }
        break;

    case Qt::Key_Space:
        if(timer.isActive()) {
            timer.stop();
            _pause->setEnabled(false);
            _continue->setEnabled(true);
        }else {
       timer.start();
       _pause->setEnabled(true);
       _continue->setEnabled(false);
      }
        break;
    default:
        break;
    }
}

void GameWidget::mousePressEvent(QMouseEvent *e){
    QPoint pointer;
    int axisX,axisY;
    if (e->button()==Qt::LeftButton){
        pointer=e->localPos().toPoint();
        axisX=pointer.x();
        axisY=pointer.y();
        int x=(axisX-10)/Label_Size;
        int y=(axisY-10)/Label_Size;
        map_label[x][y]->type=border_label;
        map_label[x][y]->label->setStyleSheet("background:black");
        map_label[x][y]->label->show();
    }
    else if (e->button()==Qt::RightButton){
        pointer=e->localPos().toPoint();
        axisX=pointer.x();
        axisY=pointer.y();
        int x=(axisX-10)/Label_Size;
        int y=(axisY-10)/Label_Size;
        map_label[x][y]->type=bg_label;
        map_label[x][y]->label->setStyleSheet("background:gray");
        map_label[x][y]->label->hide();
    }
}

void GameWidget::startGame() {
    initBorder();
    initSnake();
    moveSnake();
    createFood();
    timer.start(speed);
    _start->setDisabled(true);
}

void GameWidget::pauseGame(){
    timer.stop();
    _pause->setEnabled(false);
    _continue->setEnabled(true);
}

void GameWidget::continueGame(){
    timer.start();
    _continue->setEnabled(false);
    _pause->setEnabled(true);
}

void GameWidget::gameOver(){
    QMessageBox::information(this,"Oh!No!","Game is over! Do you want to try again?",QMessageBox::Ok);
    for(int x = 0; x < MAX_X-1; x++) {
        for(int y = 1; y < MAX_Y-1; y++) {
            this->map_label[x][y]->type = bg_label;
            this->map_label[x][y]->label->hide();
        }
    }
    initBorder();
    timer.stop();
    _start->setEnabled(true);
}

void GameWidget::enableCreate(){
    QMouseEvent *event;
    mousePressEvent(event);
}

void GameWidget::snakeMoveSlots(){
    moveSnake();
}
void GameWidget::startGameSlots(){
    startGame();
}
void GameWidget::pauseGameSlots(){
    pauseGame();
}
void GameWidget::continueGameSlots(){
    continueGame();
}
void GameWidget::enableCreateSlots(){
    enableCreate();
}
GameWidget::~GameWidget()
{
    delete ui;
}
