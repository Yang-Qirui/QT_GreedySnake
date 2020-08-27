#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1025,900);
    this->setWindowTitle("Yqr's Greedy Snake");
    this->setWindowIcon(QIcon(":/icon/icon/57994126.png"));
    ui->game->setParent(this);
    ui->_pause->setEnabled(false);
    ui->_continue->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->pause->setEnabled(false);
    ui->con->setEnabled(false);
    ui->restart->setEnabled(false);
    ui->save->setEnabled(false);
    ui->lcd->display(0);
    ui->lcd1->display(0);
    QObject::connect(ui->game,SIGNAL(gameOverSignal()),this,SLOT(gameOverSlots()));
    QObject::connect(ui->game,SIGNAL(displayStepSignal(int)),this,SLOT(displayStepSlots(int)));
    QObject::connect(ui->game,SIGNAL(displayScoreSignal(int)),this,SLOT(displayScoreSlots(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on__start_triggered()
{
    int speed= ui->speed->value();
    ui->speed->setEnabled(false);
    ui->game->startGame(qLn(10.5-speed)*50);
    ui->_start->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_continue->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(false);
    ui->_pause->setEnabled(true);
    ui->start->setEnabled(false);
    ui->restart->setEnabled(false);
    ui->con->setEnabled(false);
    ui->save->setEnabled(false);
    ui->load->setEnabled(false);
    ui->pause->setEnabled(true);
}

void MainWindow::on__pause_triggered()
{
    ui->game->pauseGame();
    ui->_save->setEnabled(true);
    ui->_pause->setEnabled(false);
    ui->_continue->setEnabled(true);
    ui->_restart->setEnabled(true);
    ui->save->setEnabled(true);
    ui->pause->setEnabled(false);
    ui->con->setEnabled(true);
    ui->restart->setEnabled(true);
}

void MainWindow::on__save_triggered()
{
    ui->game->saveGame();
}

void MainWindow::on__continue_triggered()
{
    int speed= ui->speed->value();
    ui->game->continueGame(qLn(10.5-speed)*50);
    ui->_continue->setEnabled(false);
    ui->_pause->setEnabled(true);
    ui->con->setEnabled(false);
    ui->pause->setEnabled(true);
}

void MainWindow::on__restart_triggered()
{
    ui->game->restartGame();
    ui->_continue->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(true);
    ui->_start->setEnabled(true);
    ui->con->setEnabled(false);
    ui->restart->setEnabled(false);
    ui->save->setEnabled(false);
    ui->load->setEnabled(true);
    ui->start->setEnabled(true);
}

void MainWindow::on__load_triggered()
{
    ui->game->loadGame();
    ui->_continue->setEnabled(true);
    ui->con->setEnabled(true);
}

void MainWindow::on__quit_triggered()
{
    this->close();
}

void MainWindow::gameOverSlots(){
    ui->_start->setEnabled(false);
    ui->_pause->setEnabled(false);
    ui->_continue->setEnabled(false);
    ui->_load->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_restart->setEnabled(true);
    ui->start->setEnabled(false);
    ui->pause->setEnabled(false);
    ui->con->setEnabled(false);
    ui->load->setEnabled(false);
    ui->save->setEnabled(false);
    ui->restart->setEnabled(true);
    ui->speed->setEnabled(true);
}

void MainWindow::on_start_clicked()
{
    int speed= ui->speed->value();
    ui->speed->setEnabled(false);
    ui->game->startGame(qLn(10.5-speed)*50);
    ui->_start->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_continue->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(false);
    ui->_pause->setEnabled(true);
    ui->start->setEnabled(false);
    ui->restart->setEnabled(false);
    ui->con->setEnabled(false);
    ui->save->setEnabled(false);
    ui->load->setEnabled(false);
    ui->pause->setEnabled(true);
}

void MainWindow::on_load_clicked()
{
    ui->game->loadGame();
    ui->_continue->setEnabled(true);
    ui->con->setEnabled(true);
}

void MainWindow::on_pause_clicked()
{
    ui->game->pauseGame();
    ui->_save->setEnabled(true);
    ui->_pause->setEnabled(false);
    ui->_continue->setEnabled(true);
    ui->_restart->setEnabled(true);
    ui->save->setEnabled(true);
    ui->pause->setEnabled(false);
    ui->con->setEnabled(true);
    ui->restart->setEnabled(true);
}

void MainWindow::on_con_clicked()
{
    int speed= ui->speed->value();
    ui->game->continueGame(qLn(10.5-speed)*50);
    ui->_continue->setEnabled(false);
    ui->_pause->setEnabled(true);
    ui->con->setEnabled(false);
    ui->pause->setEnabled(true);
}

void MainWindow::on_restart_clicked()
{
    ui->game->restartGame();
    ui->speed->setEnabled(true);
    ui->_continue->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(true);
    ui->_start->setEnabled(true);
    ui->con->setEnabled(false);
    ui->restart->setEnabled(false);
    ui->save->setEnabled(false);
    ui->load->setEnabled(true);
    ui->start->setEnabled(true);
}

void MainWindow::on_save_clicked()
{
     ui->game->saveGame();
}

void MainWindow::on_quit_clicked()
{
    this->close();
}

void MainWindow::displayStepSlots(int step){
    ui->lcd->display(step);
}

void MainWindow::displayScoreSlots(int score){
    ui->lcd1->display(score);
}
