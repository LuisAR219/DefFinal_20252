#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    menuWidget = new MenuWidget(this);
    stackedWidget->addWidget(menuWidget);

    connect(menuWidget, &MenuWidget::nivelSeleccionado, this, &MainWindow::onNivelSeleccionado);
    connect(menuWidget, &MenuWidget::salirJuego, this, &QMainWindow::close);

    gameWidget = new GameWidget(this);
    stackedWidget->addWidget(gameWidget);
    connect(gameWidget, &GameWidget::nivelCompletado, this, &MainWindow::onVolverAlMenu);
    connect(gameWidget, &GameWidget::nivelFallido, this, &MainWindow::onVolverAlMenu);

    mostrarMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_F11) {
        if (isFullScreen()) showNormal(); else showFullScreen();
    } else if (event->key() == Qt::Key_Escape && stackedWidget->currentWidget() == gameWidget) {
        onVolverAlMenu();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onNivelSeleccionado(int numeroNivel) {
    mostrarJuego(numeroNivel);
}

void MainWindow::onVolverAlMenu() {

    mostrarMenu();
}

void MainWindow::mostrarMenu() {
    stackedWidget->setCurrentWidget(menuWidget);
    setWindowTitle("Menú Principal - Batallas Históricas");
    resize(800, 600);
}

void MainWindow::mostrarJuego(int nivel) {
    gameWidget->inicializarNivel(nivel);
    stackedWidget->setCurrentWidget(gameWidget);
    setWindowTitle(QString("Nivel %1 - Batallas Históricas").arg(nivel));
}
