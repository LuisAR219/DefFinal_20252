#include "EntidadJuego.h"

EntidadJuego::EntidadJuego(QObject* parent)
    : QObject(parent), posicion(0, 0), velocidad(0, 0), vida(100.0f), tipo(NEUTRAL)
{}

QVector2D EntidadJuego::getPosicion() const {
    return posicion;
}

void EntidadJuego::setPosicion(const QVector2D& pos) {
    posicion = pos;
}

QVector2D EntidadJuego::getVelocidad() const {
    return velocidad;
}

void EntidadJuego::setVelocidad(const QVector2D& vel) {
    velocidad = vel;
}

float EntidadJuego::getVida() const {
    return vida;
}

void EntidadJuego::setVida(float v) {
    vida = v;
}

TipoEntidad EntidadJuego::getTipoEntidad() const {
    return tipo;
}

void EntidadJuego::setTipoEntidad(TipoEntidad t) {
    tipo = t;
}
