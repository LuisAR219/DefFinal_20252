#include "EntidadMovil.h"

EntidadMovil::EntidadMovil(QObject* parent)
    : EntidadJuego(parent), masa(1.0f), aceleracion(0, 0)
{}

void EntidadMovil::aplicarFuerza(const QVector2D& fuerza) {
    QVector2D nuevaAceleracion = fuerza / masa;
    aceleracion += nuevaAceleracion;
}

void EntidadMovil::aplicarFisica(float dt) {
    velocidad += aceleracion * dt;
    posicion += velocidad * dt;

    aceleracion = QVector2D(0, 0);
}

float EntidadMovil::getMasa() const {
    return masa;
}

void EntidadMovil::setMasa(float m) {
    masa = m;
}

QVector2D EntidadMovil::getAceleracion() const {
    return aceleracion;
}

void EntidadMovil::setAceleracion(const QVector2D& a) {
    aceleracion = a;
}
