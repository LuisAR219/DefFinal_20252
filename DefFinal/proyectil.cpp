#include "proyectil.h"

Proyectil::Proyectil(QObject* parent,
                     const QVector2D& posInicial,
                     const QVector2D& velInicial,
                     float potencia,
                     float danoInicial,
                     float radioColisionInicial)
    : EntidadMovil(parent, posInicial, 0.1f, radioColisionInicial, ENEMIGO),
    daño(danoInicial)
{
    setVelocidad(velInicial + velInicial.normalized() * potencia);
}

void Proyectil::actualizar(float dt) {
    aplicarFisica(dt);
}

bool Proyectil::colisionaCon(const EntidadJuego* otra) const {
    float dist = (getPosicion() - otra->getPosicion()).length();
    return dist < (getRadioColision() + otra->getRadioColision());
}

void Proyectil::lanzar(const QVector2D& direccion, float potencia) {
    setVelocidad(direccion.normalized() * potencia);
}

void Proyectil::pintar(QPainter* pintor) {
    pintor->setBrush(Qt::red);
    pintor->setPen(Qt::NoPen);
    pintor->drawEllipse(QPointF(getPosicion().x(), getPosicion().y()),
                        getRadioColision(), getRadioColision());
}
