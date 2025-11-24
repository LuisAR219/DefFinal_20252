#include "gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QDateTime>
#include "nivel1.h"

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    nivelActual(nullptr),
    jugador(nullptr),
    wPresionado(false),
    sPresionado(false),
    aPresionado(false),
    dPresionado(false),
    dt(0.016f),
    ultimoTiempo(0)
{
    setFocusPolicy(Qt::StrongFocus);

    connect(&temporizadorPantalla, &QTimer::timeout, this, &GameWidget::onFrameUpdate);
    temporizadorPantalla.start(16);

    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

GameWidget::~GameWidget()
{
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }
}

void GameWidget::iniciarNivel1()
{
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }

    nivelActual = new Nivel1(this);
    nivelActual->inicializar();

    jugador = nullptr;
    for (EntidadJuego* e : nivelActual->getEntidades()) {
        jugador = dynamic_cast<TanqueJugador*>(e);
        if (jugador) break;
    }

    if (!jugador)
        qWarning() << "No se encontró TanqueJugador en Nivel1()";

    wPresionado = sPresionado = aPresionado = dPresionado = false;

    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

void GameWidget::onFrameUpdate()
{
    if (!nivelActual) return;

    qint64 ahora = QDateTime::currentMSecsSinceEpoch();
    dt = (ahora - ultimoTiempo) / 1000.0f;
    ultimoTiempo = ahora;

    if (dt > 0.05f) dt = 0.05f;

    actualizarControlesJugador();

    update();
}

void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // ---- FONDO ----
    static QPixmap fondo(":/imagenes/fondo.png");
    painter.drawPixmap(rect(), fondo);

    // ---- ENTIDADES ----
    if (nivelActual)
    {
        for (EntidadJuego* e : nivelActual->getEntidades())
            e->pintar(&painter);
    }

    // ---- HUD ----
    if (jugador)
    {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14, QFont::Bold));

        // VIDA
        painter.drawText(12, 25,
                         "Vida: " + QString::number(jugador->obtenerVida()));

        // DISTANCIA
        float objetivo = 0.0f;
        if (auto n1 = dynamic_cast<Nivel1*>(nivelActual))
            objetivo = n1->obtenerDistanciaObjetivo();

        painter.drawText(12, 50,
                         "Distancia: " +
                             QString::number(jugador->obtenerDistanciaRecorrida(), 'f', 0) +
                             " / " + QString::number(objetivo, 'f', 0));
    }

    // ---- MENSAJES FINALES ----
    if (auto n1 = dynamic_cast<Nivel1*>(nivelActual))
    {
        if (n1->nivelCompletado())
        {
            painter.setFont(QFont("Arial", 28, QFont::Black));

            if (jugador->obtenerVida() <= 0)
                painter.drawText(width()/2 - 100, height()/2, "GAME OVER");
            else
                painter.drawText(width()/2 - 150, height()/2, "NIVEL COMPLETADO");
        }
    }
}

void GameWidget::actualizarControlesJugador()
{
    if (!jugador) return;

    if (wPresionado) jugador->teclaPresionada(Qt::Key_W);
    else             jugador->teclaLiberada(Qt::Key_W);

    if (sPresionado) jugador->teclaPresionada(Qt::Key_S);
    else             jugador->teclaLiberada(Qt::Key_S);

    if (aPresionado) jugador->teclaPresionada(Qt::Key_A);
    else             jugador->teclaLiberada(Qt::Key_A);

    if (dPresionado) jugador->teclaPresionada(Qt::Key_D);
    else             jugador->teclaLiberada(Qt::Key_D);
}

void GameWidget::keyPressEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat()) return;

    switch (evento->key()) {
    case Qt::Key_W: wPresionado = true; break;
    case Qt::Key_S: sPresionado = true; break;
    case Qt::Key_A: aPresionado = true; break;
    case Qt::Key_D: dPresionado = true; break;
    default:
        QWidget::keyPressEvent(evento);
        return;
    }

    if (jugador) jugador->teclaPresionada(evento->key());
}

void GameWidget::keyReleaseEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat()) return;

    switch (evento->key()) {
    case Qt::Key_W: wPresionado = false; break;
    case Qt::Key_S: sPresionado = false; break;
    case Qt::Key_A: aPresionado = false; break;
    case Qt::Key_D: dPresionado = false; break;
    default:
        QWidget::keyReleaseEvent(evento);
        return;
    }

    if (jugador) jugador->teclaLiberada(evento->key());
}
