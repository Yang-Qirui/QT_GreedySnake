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
#include <QFile>
#include <QFileDialog>
#include <QRegExp>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    initGame();
    initBorder();
    initSnake();
    canCreat=true;
    memset(clicked,true,sizeof (clicked));
    pressed=false;
    canMove=false;
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(snakeMoveSlots()));
}

void GameWidget::initGame(){
    steps=0;
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
        map_label[initX+i][initY]->label->setStyleSheet("background:green;border:1px solid rgb(240,240,240)");
        map_label[initX+i][initY]->label->show();
        snake.append(map_label[initX+i][initY]);
    }
    snake[snake.length()-1]->label->setStyleSheet("background:green;border-radius:"+QString::number(Label_Size/2));
}

void GameWidget::moveSnake(){
    steps++;
    emit displayStepSignal(steps);
    tail=snake.at(0);
    head=snake.at(snake.length()-1);
    Snake* tmp=map_label[head->x+dX][head->y+dY];
    head->label->setStyleSheet("background:green;border:1px solid rgb(240,240,240)");
    tmp->label->setStyleSheet("background:green;border-radius:"+QString::number(Label_Size/2));
    tmp->label->show();
    if (tmp->type==border_label||tmp->type==snake_label){
        gameOver();
    }
    else{
        if (tmp->type==food_label){
            scores++;
            emit displayScoreSignal(scores);
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
   if (canMove){
    switch(e->key()) {
    case Qt::Key_Left:
        if(dX != -1){
            dX = -1;
            dY = 0;
        }
        break;
    case Qt::Key_Right:
        if (!pressed){
            timer.start(speed);
            pressed=true;
        }
        if(dX != 1){
            dX = 1;
            dY = 0;
        }
        break;
    case Qt::Key_Up:
        if(dY != -1){
            dX = 0;
            dY = -1;
            if (!pressed)timer.start(speed);
            pressed=true;
            }
        break;
    case Qt::Key_Down:
        if(dY != 1){
            dX = 0;
            dY = 1;
            if (!pressed)timer.start(speed);
            pressed=true;
            }
        break;
    default:
        break;
    }
   }
}

void GameWidget::mousePressEvent(QMouseEvent *e){
    QPoint pointer;
    int axisX,axisY;
    if (!canCreat) return;
    else{
        if (e->button()==Qt::LeftButton){
            pointer=e->localPos().toPoint();
            axisX=pointer.x();
            axisY=pointer.y();
            int x=(axisX-10)/Label_Size;
            int y=(axisY-10)/Label_Size;
            if (clicked[x][y]==true){
                if (map_label[x][y]->type!=snake_label&&x!=MAX_X-1&&x!=0&&y!=MAX_Y-1&&y!=0){
                    map_label[x][y]->type=border_label;
                    map_label[x][y]->label->setStyleSheet("background:black");
                    map_label[x][y]->label->show();
                    clicked[x][y]=false;
                }
             }
             else{
                if (map_label[x][y]->type!=snake_label&&x!=MAX_X-1&&x!=0&&y!=MAX_Y-1&&y!=0){
                    map_label[x][y]->type=bg_label;
                    map_label[x][y]->label->setStyleSheet("background:gray");
                    map_label[x][y]->label->hide();
                    clicked[x][y]=true;
                }
            }
        }
    }
}

void GameWidget::startGame(double movespeed) {
    steps=0;
    scores=0;
    canCreat=false;
    canMove=true;
    speed=movespeed;
    emit displayStepSignal(steps);
    emit displayScoreSignal(scores);
    initSnake();
//    moveSnake();
    createFood();
}

void GameWidget::restartGame(){
    canCreat=true;
    steps=0;
    scores=0;
    emit displayStepSignal(0);
    emit displayScoreSignal(0);
    timer.stop();
    for(int x = 0; x < MAX_X; x++) {
        for(int y = 0; y < MAX_Y; y++) {
            map_label[x][y]->label->setStyleSheet("background:gray");
            map_label[x][y]->type = bg_label;
            map_label[x][y]->label->hide();
        }
    }
    initSnake();
    initBorder();
}

void GameWidget::pauseGame(){
    timer.stop();
}

void GameWidget::continueGame(double movespeed){
    timer.start(movespeed);
}

void GameWidget::gameOver(){
    QMessageBox::information(this,"Oh!No!","Game is over! Do you want to try again?",QMessageBox::Ok);
    for(int x = 0; x < MAX_X; x++) {
        for(int y = 0; y < MAX_Y; y++) {
            map_label[x][y]->label->setStyleSheet("background:gray");
            map_label[x][y]->type = bg_label;
            map_label[x][y]->label->hide();
            clicked[x][y]=false;
        }
    }
    canMove=false;
    pressed=false;
    initSnake();
    initBorder();
    timer.stop();
    emit gameOverSignal();
}

void GameWidget::saveGame(){
    timer.stop();
    QString snakeInfo,borderInfo,foodInfo;
    for (int i=0;i<snake.length();i++){
        snakeInfo+=QString::number(snake[i]->x);
        snakeInfo+=" ";
        snakeInfo+=QString::number(snake[i]->y);
        snakeInfo+=" ";
    }
    for (int x=0;x<MAX_X;x++){
        for (int y=0;y<MAX_Y;y++){
            if (map_label[x][y]->type==border_label){
                borderInfo+=QString::number(x);
                borderInfo+=" ";
                borderInfo+=QString::number(y);
                borderInfo+=" ";
            }
            else if (map_label[x][y]->type==food_label){
                foodInfo+=QString::number(x);
                foodInfo+=" ";
                foodInfo+=QString::number(y);
                foodInfo+=" ";
            }
        }
    }
    QString info=snakeInfo+'\n'+borderInfo+'\n'+foodInfo+'\n'+QString::number(steps)+'\n'+QString::number(scores);
    QString path=QFileDialog::getSaveFileName(this,"save","../","TXT (*.snakesavedata)");
    if (!path.isEmpty()){
        QFile file;
        file.setFileName(path);
        bool opened=file.open(QIODevice::WriteOnly);
        if (opened){
            file.write(info.toUtf8().data());
            QMessageBox::information(this,"Finished!","Save Successfully",QMessageBox::Ok);
        }
        file.close();
    }
}

void GameWidget::loadGame(){
    QByteArray array;
    QString path=QFileDialog::getOpenFileName(this,"open","../","TXT (*.snakesavedata)");
    if (path.isEmpty()==false){
        QFile file(path);
        bool opened=file.open(QIODevice::ReadOnly);
        if (opened){
            array=file.readLine();
            QList<QByteArray> snakelist=array.split(' ');
            array=file.readLine();
            QList<QByteArray> borderlist=array.split(' ');
            array=file.readLine();
            QList<QByteArray> foodlist=array.split(' ');
            array=file.readLine();
            steps=array.toInt();
            array=file.readLine();
            scores=array.toInt();
            for(int x = 0; x < MAX_X; x++) {
                for(int y = 0; y < MAX_Y; y++) {
                    map_label[x][y]->label->setStyleSheet("background:gray");
                    map_label[x][y]->type = bg_label;
                    map_label[x][y]->label->hide();
                }
            }
            snake.clear();
            for (int i=0;i<snakelist.length()-1;i+=2){
                if (snakelist[i]!="\n"&&snakelist[i]!=" "){
                    map_label[snakelist[i].toInt()][snakelist[i+1].toInt()]->type=snake_label;
                    map_label[snakelist[i].toInt()][snakelist[i+1].toInt()]->label->setStyleSheet("background:green;border:1px solid rgb(240,240,240)");
                    map_label[snakelist[i].toInt()][snakelist[i+1].toInt()]->label->show();
                    snake.push_back(map_label[snakelist[i].toInt()][snakelist[i+1].toInt()]);
                }
            }
            snake[snake.length()-1]->label->setStyleSheet("background:green;border-radius:"+QString::number(Label_Size/2));
            for (int i=0;i<borderlist.length()-1;i+=2){
                if (borderlist[i]!="\n"&&borderlist[i]!=" "){
                    map_label[borderlist[i].toInt()][borderlist[i+1].toInt()]->type=border_label;
                    map_label[borderlist[i].toInt()][borderlist[i+1].toInt()]->label->setStyleSheet("background:black");
                    map_label[borderlist[i].toInt()][borderlist[i+1].toInt()]->label->show();
                }
            }
            for (int i=0;i<foodlist.length()-1;i+=2){
                if (foodlist[i]!="\n"&&foodlist[i]!=" "){
                    map_label[foodlist[i].toInt()][foodlist[i+1].toInt()]->type=food_label;
                    map_label[foodlist[i].toInt()][foodlist[i+1].toInt()]->label->setStyleSheet("background:red");
                    map_label[foodlist[i].toInt()][foodlist[i+1].toInt()]->label->show();
                }
            }
            if (snake[snake.length()-1]->x==snake[snake.length()-2]->x&&snake[snake.length()-1]->y>snake[snake.length()-2]->y) {dX=0;dY=1;qDebug()<<"down";}
            if (snake[snake.length()-1]->x==snake[snake.length()-2]->x&&snake[snake.length()-1]->y<snake[snake.length()-2]->y) {dX=0;dY=-1;qDebug()<<"up";}
            if (snake[snake.length()-1]->x>snake[snake.length()-2]->x&&snake[snake.length()-1]->y==snake[snake.length()-2]->y) {dX=1;dY=0;qDebug()<<"right";}
            if (snake[snake.length()-1]->x<snake[snake.length()-2]->x&&snake[snake.length()-1]->y==snake[snake.length()-2]->y) {dX=-1;dY=0;qDebug()<<"left";}
            emit displayStepSignal(steps);
            emit displayScoreSignal(scores);
            QMessageBox::information(this,"Finished!","Load Successfully",QMessageBox::Ok);
        }
        file.close();
    }

}

void GameWidget::quitGame(){
    this->parentWidget()->close();
}

void GameWidget::snakeMoveSlots(){
    moveSnake();
}
GameWidget::~GameWidget()
{
    delete ui;
}
