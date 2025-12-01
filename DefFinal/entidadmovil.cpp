#include "EntidadMovil.h"

EntidadMovil::EntidadMovil(QObject* parent,
                           const QVector2D& posInicial,
                           float masaInicial,
                           float radioColisionInicial,
                           TipoEntidad tipoInicial)
    : EntidadJuego(parent, posInicial, masaInicial, radioColisionInicial, tipoInicial),
    aceleracion(0, 0)
{}

void EntidadMovil::aplicarFuerza(const QVector2D& fuerza) {
    QVector2D nuevaAceleracion = fuerza / getMasa();
    aceleracion += nuevaAceleracion;
}

void EntidadMovil::aplicarFisica(float dt) {
    setVelocidad(getVelocidad() + aceleracion * dt);
    setPosicion(getPosicion() + getVelocidad() * dt);
    aceleracion = QVector2D(0, 0);
}
