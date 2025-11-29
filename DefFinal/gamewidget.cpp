#include "gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QDateTime>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    nivelActual(nullptr),
    jugador(nullptr),
    nivelActualN2(nullptr),
    canonJugador(nullptr),
    wPresionado(false),
    sPresionado(false),
    aPresionado(false),
    dPresionado(false),
    teclaA_N2(false),
    teclaD_N2(false),
    teclaEspacio_N2(false),
    dt(0.016f),
    ultimoTiempo(0)
{
    setFocusPolicy(Qt::StrongFocus);

    connect(&temporizadorPantalla, &QTimer::timeout,
            this, &GameWidget::onFrameUpdate);

    temporizadorPantalla.start(16);

    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

GameWidget::~GameWidget()
{
    if (nivelActual)
        delete nivelActual;

    if (nivelActualN2)
        delete nivelActualN2;
}

//
// ===============================
//       INICIAR NIVEL 1
// ===============================
//
void GameWidget::iniciarNivel1()
{
    if (nivelActual)
        delete nivelActual;

    if (nivelActualN2)
        delete nivelActualN2;

    nivelActualN2 = nullptr;
    canonJugador = nullptr;

    nivelActual = new Nivel1(this);
    nivelActual->inicializar();

    jugador = nullptr;
    for (EntidadJuego* e : nivelActual->getEntidades())
    {
        jugador = dynamic_cast<TanqueJugador*>(e);
        if (jugador) break;
    }

    if (!jugador)
        qWarning() << "No se encontró TanqueJugador en Nivel1";

    wPresionado = sPresionado = aPresionado = dPresionado = false;
    ultimoTiempo = QDateTime::currentMSecsSinceEpoch();
}

//
// ===============================
//       INICIAR NIVEL 2
// ===============================
//
void GameWidget::iniciarNivel2()
{
    if (nivelActual)
        delete nivelActual;

    if (nivelActualN2)
        delete nivelActualN2;

    nivelActual = nullptr;
    jugador = nullptr;

    nivelActualN2 = new Nivel2(this);
    nivelActualN2->inicializar();

    canonJugador = nivelActualN2->obtenerCanon();
    if (!canonJugador)
        qWarning() << "No se encontró Canon del jugador en Nivel2";

    teclaA_N2 = teclaD_N2 = teclaEspacio_N2 = false;
}

//
// ===============================
//       ACTUALIZACIÓN FRAME
// ===============================
//
void GameWidget::onFrameUpdate()
{
    qint64 ahora = QDateTime::currentMSecsSinceEpoch();
    dt = (ahora - ultimoTiempo) / 1000.0f;
    ultimoTiempo = ahora;
    if (dt > 0.05f) dt = 0.05f;

    if (nivelActual)          // Nivel 1
        actualizarControlesJugador();

    if (nivelActualN2)        // Nivel 2
    {
        actualizarControlesNivel2();
        nivelActualN2->actualizar(dt);
    }

    update();
}

//
// ===============================
//           DIBUJADO
// ===============================
//
void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // Fondo
    painter.fillRect(rect(), QColor(10, 30, 60));

    // ---- NIVEL 1 ----
    if (nivelActual)
    {
        for (EntidadJuego* e : nivelActual->getEntidades())
            e->pintar(&painter);
    }

    // ---- NIVEL 2 ----
    if (nivelActualN2)
        nivelActualN2->pintar(&painter);
}

//
// ===============================
//   CONTROLES: NIVEL 1
// ===============================
//
void GameWidget::actualizarControlesJugador()
{
    if (!jugador) return;

    if (wPresionado) jugador->teclaPresionada(Qt::Key_W);
    else jugador->teclaLiberada(Qt::Key_W);

    if (sPresionado) jugador->teclaPresionada(Qt::Key_S);
    else jugador->teclaLiberada(Qt::Key_S);

    if (aPresionado) jugador->teclaPresionada(Qt::Key_A);
    else jugador->teclaLiberada(Qt::Key_A);

    if (dPresionado) jugador->teclaPresionada(Qt::Key_D);
    else jugador->teclaLiberada(Qt::Key_D);
}

//
// ===============================
//   CONTROLES: NIVEL 2
// ===============================
//
void GameWidget::actualizarControlesNivel2()
{
    if (!canonJugador) return;

    if (teclaA_N2)
        canonJugador->procesarTecla(Qt::Key_A, true);
    else
        canonJugador->procesarTecla(Qt::Key_A, false);

    if (teclaD_N2)
        canonJugador->procesarTecla(Qt::Key_D, true);
    else
        canonJugador->procesarTecla(Qt::Key_D, false);

    if (teclaEspacio_N2)
        canonJugador->procesarTecla(Qt::Key_Space, true);
    else
        canonJugador->procesarTecla(Qt::Key_Space, false);
}

//
// ===============================
//       ENTRADA DE TECLAS
// ===============================
//
void GameWidget::keyPressEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat())
        return;

    // ----- NIVEL 1 -----
    if (nivelActual)
    {
        switch (evento->key()) {
        case Qt::Key_W: wPresionado = true; break;
        case Qt::Key_S: sPresionado = true; break;
        case Qt::Key_A: aPresionado = true; break;
        case Qt::Key_D: dPresionado = true; break;
        }
    }

    // ----- NIVEL 2 -----
    if (nivelActualN2)
    {
        switch (evento->key()) {
        case Qt::Key_A: teclaA_N2 = true; break;
        case Qt::Key_D: teclaD_N2 = true; break;
        case Qt::Key_Space: teclaEspacio_N2 = true; break;
        }
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat())
        return;

    // ----- NIVEL 1 -----
    if (nivelActual)
    {
        switch (evento->key()) {
        case Qt::Key_W: wPresionado = false; break;
        case Qt::Key_S: sPresionado = false; break;
        case Qt::Key_A: aPresionado = false; break;
        case Qt::Key_D: dPresionado = false; break;
        }
    }

    // ----- NIVEL 2 -----
    if (nivelActualN2)
    {
        switch (evento->key()) {
        case Qt::Key_A: teclaA_N2 = false; break;
        case Qt::Key_D: teclaD_N2 = false; break;
        case Qt::Key_Space: teclaEspacio_N2 = false; break;
        }
    }
}
