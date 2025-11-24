#include "tanquejugador.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>

TanqueJugador::TanqueJugador(QObject* parent,
                             const QVector2D& posicionInicial)
    : EntidadJuego(parent, posicionInicial, 50.0f, 22.0f, TipoEntidad::JUGADOR),
    velocidadY(0.0f),
    velocidadX(220.0f),
    distanciaRecorrida(0.0f),
    minX(0.0f), maxX(800.0f),
    minY(0.0f), maxY(600.0f),
    vida(100.0f)
{
    sprite = QPixmap(":/imagenes/tanque.png");
    anchoSprite = 48;
    altoSprite  = 48;
}

static const float aceleracionY  = 280.0f;
static const float friccionY     = 0.90f;
static const float velocidadMaxY = 230.0f;
static const float frenoY        = 320.0f;

void TanqueJugador::establecerLimites(float minX_, float maxX_, float minY_, float maxY_)
{
    minX = minX_;
    maxX = maxX_;
    minY = minY_;
    maxY = maxY_;
}

void TanqueJugador::teclaPresionada(int key)
{
    teclasActivas.insert(key);
}

void TanqueJugador::teclaLiberada(int key)
{
    teclasActivas.remove(key);
}

void TanqueJugador::actualizar(float dt)
{
    QVector2D pos = obtenerPosicion();

    if (teclasActivas.contains(Qt::Key_A))
        pos.setX(pos.x() - velocidadX * dt);

    if (teclasActivas.contains(Qt::Key_D))
        pos.setX(pos.x() + velocidadX * dt);

    if (teclasActivas.contains(Qt::Key_W))
        velocidadY -= aceleracionY * dt;

    if (teclasActivas.contains(Qt::Key_S))
        velocidadY += frenoY * dt;

    if (!teclasActivas.contains(Qt::Key_W))
        velocidadY *= friccionY;

    velocidadY = qBound(-velocidadMaxY, velocidadY, velocidadMaxY);

    pos.setY(pos.y() + velocidadY * dt);

    if (pos.x() < minX) pos.setX(minX);
    if (pos.x() > maxX) pos.setX(maxX);

    if (pos.y() < minY) { pos.setY(minY); velocidadY = 0; }
    if (pos.y() > maxY) { pos.setY(maxY); velocidadY = 0; }

    establecerPosicion(pos);

    distanciaRecorrida += qAbs(velocidadY * dt);
}

void TanqueJugador::aplicarFuerza(const QVector2D& fuerza)
{
    velocidadY += fuerza.y();
}

bool TanqueJugador::colisionaCon(const EntidadJuego* otra) const
{
    float dist = (otra->obtenerPosicion() - posicion).length();
    return dist < (radioColision + otra->obtenerRadioColision());
}

void TanqueJugador::pintar(QPainter* p)
{
    QRectF rect(posicion.x() - anchoSprite/2,
                posicion.y() - altoSprite/2,
                1.5*anchoSprite,
                1.5*altoSprite);

    p->drawPixmap(rect.toRect(), sprite);

}
