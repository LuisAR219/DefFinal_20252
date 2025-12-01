#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "menuwidget.h"
#include "gamewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNivelSeleccionado(int numeroNivel);
    void onVolverAlMenu();

private:
    Ui::MainWindow *ui;
    QStackedWidget* stackedWidget;
    MenuWidget* menuWidget;
    GameWidget* gameWidget;

    void mostrarMenu();
    void mostrarJuego(int nivel);
};

#endif
