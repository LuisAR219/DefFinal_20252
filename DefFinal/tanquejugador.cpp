#include "tanquejugador.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>

// ------------------------------------------------------------
// CONSTRUCTOR
// ------------------------------------------------------------
TanqueJugador::TanqueJugador(QObject* parent,
                             const QVector2D& posicionInicial)
    : EntidadJuego(parent, posicionInicial, 50.0f, 20.0f, TipoEntidad::JUGADOR),
    velocidadY(0.0f),
    velocidadX(220.0f),
    distanciaRecorrida(0.0f),
    minX(0.0f), maxX(800.0f),
    minY(0.0f), maxY(600.0f),
    vida(100.0f)
{
}

// ======================================================
//                AJUSTA ESTOS VALORES AQUÍ
// ======================================================
static const float aceleracionY  = 280.0f;
static const float friccionY     = 0.90f;
static const float velocidadMaxY = 230.0f;
static const float frenoY        = 320.0f;
// ======================================================


// ------------------------------------------------------------
// LIMITE DE MOVIMIENTO
// ------------------------------------------------------------
void TanqueJugador::establecerLimites(float minX_, float maxX_, float minY_, float maxY_)
{
    minX = minX_;
    maxX = maxX_;
    minY = minY_;
    maxY = maxY_;
}

// ------------------------------------------------------------
// TECLAS
// ------------------------------------------------------------
void TanqueJugador::teclaPresionada(int key)
{
    teclasActivas.insert(key);
}

void TanqueJugador::teclaLiberada(int key)
{
    teclasActivas.remove(key);
}


// ------------------------------------------------------------
// ACTUALIZAR FÍSICAS
// ------------------------------------------------------------
void TanqueJugador::actualizar(float dt)
{
    QVector2D pos = obtenerPosicion();

    // Movimiento horizontal instantáneo
    if (teclasActivas.contains(Qt::Key_A))
        pos.setX(pos.x() - velocidadX * dt);

    if (teclasActivas.contains(Qt::Key_D))
        pos.setX(pos.x() + velocidadX * dt);

    // Aceleración vertical
    if (teclasActivas.contains(Qt::Key_W))
        velocidadY -= aceleracionY * dt;

    // Freno con S
    if (teclasActivas.contains(Qt::Key_S))
        velocidadY += frenoY * dt;

    // Fricción si NO se presiona W
    if (!teclasActivas.contains(Qt::Key_W))
        velocidadY *= friccionY;

    // Limitar velocidad
    velocidadY = qBound(-velocidadMaxY, velocidadY, velocidadMaxY);

    // Aplicar movimiento
    pos.setY(pos.y() + velocidadY * dt);

    // Límites
    if (pos.x() < minX) pos.setX(minX);
    if (pos.x() > maxX) pos.setX(maxX);

    if (pos.y() < minY) { pos.setY(minY); velocidadY = 0; }
    if (pos.y() > maxY) { pos.setY(maxY); velocidadY = 0; }

    // Aplicar posición final
    establecerPosicion(pos);

    distanciaRecorrida += qAbs(velocidadY * dt);
}


// ------------------------------------------------------------
// aplicarFuerza — implementación mínima
// ------------------------------------------------------------
void TanqueJugador::aplicarFuerza(const QVector2D& fuerza)
{
    // Lo dejamos simple por ahora: fuerza solo afecta velocidadY
    velocidadY += fuerza.y();
}


// ------------------------------------------------------------
// Colisión circular básica
// ------------------------------------------------------------
bool TanqueJugador::colisionaCon(const EntidadJuego* otra) const
{
    float distancia = (otra->obtenerPosicion() - posicion).length();
    return distancia < (radioColision + otra->obtenerRadioColision());
}


// ------------------------------------------------------------
// DIBUJAR EL TANQUE (provisional)
// ------------------------------------------------------------
void TanqueJugador::pintar(QPainter* p)
{
    p->setBrush(Qt::green);
    p->drawEllipse(QPointF(posicion.x(), posicion.y()), radioColision, radioColision);
}
