#include "gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QDateTime>

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

    // Timer de refresco de la pantalla
    connect(&temporizadorPantalla, &QTimer::timeout, this, &GameWidget::onFrameUpdate);
    temporizadorPantalla.start(16);

    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

GameWidget::~GameWidget()
{
    // nivelActual fue creado con parent=this en iniciarNivel1; igualmente lo liberamos
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }
}

void GameWidget::iniciarNivel1()
{
    // Eliminar nivel anterior (si existe)
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }

    // Crear nuevo nivel y dejar que él inicie su temporizador interno
    nivelActual = new Nivel1(this);
    nivelActual->inicializar();

    // Localizar puntero al jugador dentro de las entidades del nivel
    jugador = nullptr;
    for (EntidadJuego* e : nivelActual->getEntidades()) {
        jugador = dynamic_cast<TanqueJugador*>(e);
        if (jugador) break;
    }

    if (!jugador) {
        qWarning() << "GameWidget::iniciarNivel1 - No se encontró TanqueJugador en Nivel1()";
    }

    // Reset de flags temporales de entrada
    wPresionado = sPresionado = aPresionado = dPresionado = false;

    // Reset del tiempo para dt
    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

void GameWidget::onFrameUpdate()
{
    if (!nivelActual) return;

    qint64 ahora = QDateTime::currentMSecsSinceEpoch();
    dt = (ahora - ultimoTiempo) / 1000.0f;
    ultimoTiempo = ahora;

    // Capar dt para evitar saltos enormes si hubo lag
    if (dt > 0.05f) dt = 0.05f;

    // Enviamos el estado de teclas al jugador (asegura sincronía)
    actualizarControlesJugador();

    // El nivel tiene su propio temporizador (si hace cosas periódicas internamente).
    // No llamamos a nivelActual->actualizar(dt) explícitamente para evitar duplicidad.
    // Si deseas controlar el tiempo desde aquí en vez de usar temporizador interno,
    // podría llamarse aquí.

    // Repintar la pantalla
    update();
}

void GameWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fondo
    painter.fillRect(rect(), Qt::black);

    if (!nivelActual) {
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Presiona 'Iniciar Nivel 1' para comenzar");
        return;
    }

    // Pintar todas las entidades (cada entidad implementa pintar)
    for (EntidadJuego* e : nivelActual->getEntidades()) {
        e->pintar(&painter);
    }

    // HUD del jugador
    if (jugador) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.drawText(12, 24, "Vida: " + QString::number(jugador->obtenerVida(), 'f', 0));
        painter.drawText(12, 44, "Distancia: " + QString::number(jugador->obtenerDistanciaRecorrida(), 'f', 0));
    }

    // Mensaje de nivel completado
    if (nivelActual->nivelCompletado()) {
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "NIVEL COMPLETADO");
    }
}

void GameWidget::actualizarControlesJugador()
{
    if (!jugador) return;

    // Enviamos los estados actuales de las teclas a TanqueJugador
    // Usamos teclaPresionada/teclaLiberada para que TanqueJugador mantenga su propio set
    if (wPresionado)
        jugador->teclaPresionada(Qt::Key_W);
    else
        jugador->teclaLiberada(Qt::Key_W);

    if (sPresionado)
        jugador->teclaPresionada(Qt::Key_S);
    else
        jugador->teclaLiberada(Qt::Key_S);

    if (aPresionado)
        jugador->teclaPresionada(Qt::Key_A);
    else
        jugador->teclaLiberada(Qt::Key_A);

    if (dPresionado)
        jugador->teclaPresionada(Qt::Key_D);
    else
        jugador->teclaLiberada(Qt::Key_D);
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

    // Informar inmediatamente al jugador si ya fue encontrado
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
