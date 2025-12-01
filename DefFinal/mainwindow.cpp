#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"
#include <QKeyEvent>

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_F11) {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GameWidget* juego = new GameWidget(this);
    setCentralWidget(juego);
    juego->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindow::~MainWindow()
{
    delete ui;
}
