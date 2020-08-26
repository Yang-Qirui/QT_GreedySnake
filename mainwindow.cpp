#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1025,900);
    ui->game->setParent(this);
    QObject::connect(ui->game,SIGNAL(gameOverSignal()),this,SLOT(gameOverSlots()));
    QObject::connect(ui->game,SIGNAL(startGameSignal()),this,SLOT(startGameSlots()));
    QObject::connect(ui->game,SIGNAL(pauseGameSignal()),this,SLOT(pauseGameSlots()));
    QObject::connect(ui->game,SIGNAL(continueGameSignal()),this,SLOT(continueGameSlots()));
    QObject::connect(ui->game,SIGNAL(restartGameSignal()),this,SLOT(restartGameSlots()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on__start_triggered()
{
    ui->game->startGame();
    ui->_start->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_continue->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(false);
    ui->_pause->setEnabled(true);
}

void MainWindow::on__pause_triggered()
{
    ui->game->pauseGame();
    ui->_save->setEnabled(true);
    ui->_pause->setEnabled(false);
    ui->_continue->setEnabled(true);
    ui->_restart->setEnabled(true);
}

void MainWindow::on__save_triggered()
{
    ui->game->saveGame();
}

void MainWindow::on__continue_triggered()
{
    ui->game->continueGame();
    ui->_continue->setEnabled(false);
    ui->_pause->setEnabled(true);
}

void MainWindow::on__restart_triggered()
{
    ui->game->restartGame();
    ui->_continue->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(true);
    ui->_start->setEnabled(true);
}

void MainWindow::on__load_triggered()
{
    ui->game->loadGame();
    ui->_continue->setEnabled(true);
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
}

void MainWindow::startGameSlots(){
    ui->_start->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_continue->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(false);
    ui->_pause->setEnabled(true);
}

void MainWindow::pauseGameSlots(){
    ui->_save->setEnabled(true);
    ui->_pause->setEnabled(false);
    ui->_continue->setEnabled(true);
    ui->_restart->setEnabled(true);
}

void MainWindow::continueGameSlots(){
    ui->_continue->setEnabled(false);
    ui->_pause->setEnabled(true);
}

void MainWindow::restartGameSlots(){
    ui->_continue->setEnabled(false);
    ui->_restart->setEnabled(false);
    ui->_save->setEnabled(false);
    ui->_load->setEnabled(true);
    ui->_start->setEnabled(true);
}
