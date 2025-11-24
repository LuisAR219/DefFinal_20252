#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "nivel1.h"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget();

    void iniciarNivel1();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* evento) override;
    void keyReleaseEvent(QKeyEvent* evento) override;

private slots:
    void onFrameUpdate();

private:
    Nivel1* nivelActual;
    QTimer temporizadorPantalla;

    TanqueJugador* jugador;

    float dt;
    qint64 ultimoTiempo;

    // Entradas de movimiento
    bool wPresionado;
    bool sPresionado;
    bool aPresionado;
    bool dPresionado;

    // Envia los inputs al tanque (arcade)
    void actualizarControlesJugador();
};

#endif // GAMEWIDGET_H
