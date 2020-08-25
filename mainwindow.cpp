#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GameWidget* game=new GameWidget;
    ui->gameLayout->addWidget(game);
    ui->gridLayoutWidget->setFixedSize(MAX_X*Label_Size,MAX_Y*Label_Size);
    this->setFixedSize((MAX_X+7)*Label_Size,(MAX_Y+1.5)*Label_Size);
    QPushButton* startButton=new QPushButton("Start",this);
    startButton->setGeometry((MAX_X+2)*Label_Size,Label_Size,80,60);
    startButton->show();
    QObject::connect(startButton,SIGNAL(clicked()),game,SLOT(startGameSlots()));
    QObject::connect(startButton,SIGNAL(clicked()),startButton,SLOT(hide()));
    QObject::connect(game,SIGNAL(resetButton()),startButton,SLOT(show()));
    this->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

