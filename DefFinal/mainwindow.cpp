#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , juego(nullptr)
{
    ui->setupUi(this);

    juego = new GameWidget(this);

    ui->gameContainer->layout()->addWidget(juego);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnIniciarNivel1_clicked()
{
    if (juego)
        juego->iniciarNivel1();
}



