#include "Proyectil.h"

Proyectil::Proyectil(QObject* parent)
    : EntidadMovil(parent), daño(50.0f)
{}

void Proyectil::actualizar(float dt) {
    aplicarFisica(dt);
}

bool Proyectil::colisionaCon(const EntidadJuego* otra) const {
    return (posicion - otra->getPosicion()).length() < 10.0f;
}

void Proyectil::lanzar(const QVector2D& direccion, float potencia) {
    velocidad = direccion.normalized() * potencia;
}

float Proyectil::getDaño() const {
    return daño;
}

void Proyectil::setDaño(float d) {
    daño = d;
}
