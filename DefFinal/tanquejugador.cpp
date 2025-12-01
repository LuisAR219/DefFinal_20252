#include "tanquejugador.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>

TanqueJugador::TanqueJugador(QObject* parent,
                             const QVector2D& posicionInicial)
    : EntidadJuego(parent, posicionInicial, 50.0f, 22.0f, JUGADOR), // CORREGIDO
    velocidadY(0.0f),
    velocidadX(220.0f),
    distanciaRecorrida(0.0f),
    minX(0.0f),
    maxX(800.0f),
    minY(0.0f),
    maxY(600.0f)
{
    sprite = QPixmap(":/imagenes/tanque.png");
    anchoSprite = 48;
    altoSprite  = 48;
    setVida(100.0f);
}

void TanqueJugador::establecerLimites(float minX_, float maxX_, float minY_, float maxY_) {
    minX = minX_;
    maxX = maxX_;
    minY = minY_;
    maxY = maxY_;
}

void TanqueJugador::teclaPresionada(int key) {
    teclasActivas.insert(key);
}

void TanqueJugador::teclaLiberada(int key) {
    teclasActivas.remove(key);
}

void TanqueJugador::actualizar(float dt) {
    QVector2D pos = getPosicion(); // CORREGIDO

    if (teclasActivas.contains(Qt::Key_A))
        pos.setX(pos.x() - velocidadX * dt);

    if (teclasActivas.contains(Qt::Key_D))
        pos.setX(pos.x() + velocidadX * dt);

    if (teclasActivas.contains(Qt::Key_W))
        velocidadY -= 280.0f * dt;

    if (teclasActivas.contains(Qt::Key_S))
        velocidadY += 320.0f * dt;

    if (!teclasActivas.contains(Qt::Key_W))
        velocidadY *= 0.90f;

    velocidadY = qBound(-230.0f, velocidadY, 230.0f);
    pos.setY(pos.y() + velocidadY * dt);

    if (pos.x() < minX) pos.setX(minX);
    if (pos.x() > maxX) pos.setX(maxX);
    if (pos.y() < minY) { pos.setY(minY); velocidadY = 0; }
    if (pos.y() > maxY) { pos.setY(maxY); velocidadY = 0; }

    setPosicion(pos); // CORREGIDO
    distanciaRecorrida += qAbs(velocidadY * dt);
}

void TanqueJugador::aplicarFuerza(const QVector2D& fuerza) {
    velocidadY += fuerza.y();
}

bool TanqueJugador::colisionaCon(const EntidadJuego* otra) const {
    float dist = (otra->getPosicion() - getPosicion()).length(); // CORREGIDO
    return dist < (getRadioColision() + otra->getRadioColision()); // CORREGIDO
}

void TanqueJugador::pintar(QPainter* p) {
    QRectF rect(getPosicion().x() - anchoSprite/2, // CORREGIDO
                getPosicion().y() - altoSprite/2,  // CORREGIDO
                1.5*anchoSprite,
                1.5*altoSprite);
    p->drawPixmap(rect.toRect(), sprite);
}
