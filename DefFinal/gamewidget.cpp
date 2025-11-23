#include "gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>


GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    nivelActual(nullptr),
    jugador(nullptr),
    wPresionado(false),
    sPresionado(false),
    aPresionado(false),
    dPresionado(false),
    dt(0.016f)
{
    setFocusPolicy(Qt::StrongFocus);

    connect(&temporizadorPantalla, &QTimer::timeout, this, &GameWidget::onFrameUpdate);
    temporizadorPantalla.start(16);

    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

GameWidget::~GameWidget()
{
    delete nivelActual;
}

void GameWidget::iniciarNivel1()
{
    if (nivelActual)
        delete nivelActual;

    nivelActual = new Nivel1(this);
    nivelActual->inicializar();

    jugador = nullptr;
    for (EntidadJuego* e : nivelActual->getEntidades()) {
        jugador = dynamic_cast<TanqueJugador*>(e);
        if (jugador) break;
    }

    if (!jugador)
        qWarning() << "ADVERTENCIA: No se encontró TanqueJugador en Nivel1.";
}

void GameWidget::onFrameUpdate()
{
    if (!nivelActual) return;

    qint64 tiempoActual = QDateTime::currentMSecsSinceEpoch();
    dt = (tiempoActual - ultimoTiempo) / 1000.0f;
    ultimoTiempo = tiempoActual;

    if (dt > 0.05f) dt = 0.05f;

    actualizarControlesJugador();

    update();  // dispara paintEvent()
}

void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!nivelActual) {
        painter.drawText(rect(), Qt::AlignCenter, "Presiona el botón para iniciar Nivel 1");
        return;
    }

    // Pintar todas las entidades
    for (EntidadJuego* e : nivelActual->getEntidades()) {
        e->pintar(&painter);
    }

    // HUD
    if (jugador) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14, QFont::Bold));

        painter.drawText(20, 30,
                         "Vida: " + QString::number(jugador->obtenerVida()));

        painter.drawText(20, 55,
                         "Distancia: " +
                             QString::number(jugador->obtenerDistanciaRecorrida(), 'f', 0));
    }

    if (nivelActual->nivelCompletado()) {
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("Arial", 26, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "NIVEL COMPLETADO");
    }
}

void GameWidget::actualizarControlesJugador()
{
    if (!jugador) return;

    ControlTanque* cv = jugador->obtenerControlVertical();
    ControlTanque* cl = jugador->obtenerControlLateral();

    bool acelera = wPresionado;
    bool frena   = sPresionado;
    bool izq     = aPresionado;
    bool der     = dPresionado;

    cv->establecerInput(acelera, frena);
    cl->establecerInput(der, izq); // derecha = positivo
}

void GameWidget::keyPressEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat()) return;

    switch (evento->key()) {
    case Qt::Key_W: wPresionado = true; break;
    case Qt::Key_S: sPresionado = true; break;
    case Qt::Key_A: aPresionado = true; break;
    case Qt::Key_D: dPresionado = true; break;
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat()) return;

    switch (evento->key()) {
    case Qt::Key_W: wPresionado = false; break;
    case Qt::Key_S: sPresionado = false; break;
    case Qt::Key_A: aPresionado = false; break;
    case Qt::Key_D: dPresionado = false; break;
    }
}
