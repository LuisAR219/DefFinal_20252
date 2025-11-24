#include "Tanque.h"

Tanque::Tanque(QObject* parent)
    : EntidadMovil(parent), velocidadObjetivo(0), coeficienteFriccion(0.1f)
{}

void Tanque::actualizar(float dt) {
    aplicarFisica(dt);
    velocidad *= (1.0f - coeficienteFriccion);
}

bool Tanque::colisionaCon(const EntidadJuego* otra) const {
    return (posicion - otra->getPosicion()).length() < 30.0f;
}

void Tanque::atacar(const QVector2D& direccion, float potencia) {
    Proyectil* p = new Proyectil();
    p->setPosicion(this->getPosicion());
    p->lanzar(direccion, potencia);
    p->setTipoEntidad(NEUTRAL);

    emit proyectilCreado(p);
}
