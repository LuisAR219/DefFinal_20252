#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GameWidget* juego = new GameWidget(this);
    setCentralWidget(juego);
}

MainWindow::~MainWindow()
{
    delete ui;
}
