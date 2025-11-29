#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "nivel1.h"
#include "nivel2.h"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget();

    void iniciarNivel1();
    void iniciarNivel2();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* evento) override;
    void keyReleaseEvent(QKeyEvent* evento) override;

private slots:
    void onFrameUpdate();

private:

    // ================================
    //         N I V E L   1
    // ================================
    Nivel1* nivelActual;
    TanqueJugador* jugador;

    // ================================
    //         N I V E L   2
    // ================================
    Nivel2* nivelActualN2;
    Canon* canonJugador;

    bool teclaA_N2;
    bool teclaD_N2;
    bool teclaEspacio_N2;

    // ================================
    //      CONFIGURACIÓN GENERAL
    // ================================
    QTimer temporizadorPantalla;

    float dt;
    qint64 ultimoTiempo;

    // Entradas Nivel 1
    bool wPresionado;
    bool sPresionado;
    bool aPresionado;
    bool dPresionado;

    // ================================
    //        MÉTODOS INTERNOS
    // ================================
    void actualizarControlesJugador();   // Nivel 1
    void actualizarControlesNivel2();    // Nivel 2
};

#endif // GAMEWIDGET_H
