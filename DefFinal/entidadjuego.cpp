#include "entidadjuego.h"
#include <QDebug>

EntidadJuego::EntidadJuego(QObject* parent,
                           const QVector2D& posicionInicial,
                           float masaInicial,
                           float radioColisionInicial,
                           TipoEntidad tipoInicial)
    : QObject(parent),
    posicion(posicionInicial),
    velocidad(0.0f, 0.0f),
    masa(masaInicial > 0.0f ? masaInicial : 1.0f),
    radioColision(radioColisionInicial > 0.0f ? radioColisionInicial : 10.0f),
    tipoEntidad(tipoInicial)
{
    if (masaInicial <= 0.0f) {
        qWarning() << "EntidadJuego: masa inválida indicada, se usa masa = 1.0";
    }
    if (radioColisionInicial <= 0.0f) {
        qWarning() << "EntidadJuego: radio de colisión inválido, se usa radio = 10.0";
    }
}

EntidadJuego::~EntidadJuego()
{
    // Destructor virtual para permitir eliminación segura a través de punteros base
}

// ----------------------
// Getters / Setters
// ----------------------

QVector2D EntidadJuego::obtenerPosicion() const {
    return posicion;
}

void EntidadJuego::establecerPosicion(const QVector2D& pos) {
    posicion = pos;
}

QVector2D EntidadJuego::obtenerVelocidad() const {
    return velocidad;
}

void EntidadJuego::establecerVelocidad(const QVector2D& vel) {
    velocidad = vel;
}

float EntidadJuego::obtenerRadioColision() const {
    return radioColision;
}

void EntidadJuego::establecerRadioColision(float radio) {
    if (radio > 0.0f) {
        radioColision = radio;
    } else {
        qWarning() << "EntidadJuego: intento de asignar radio no válido.";
    }
}

float EntidadJuego::obtenerMasa() const {
    return masa;
}

void EntidadJuego::establecerMasa(float m) {
    if (m > 0.0f) {
        masa = m;
    } else {
        qWarning() << "EntidadJuego: intento de asignar masa no válida.";
    }
}

TipoEntidad EntidadJuego::obtenerTipo() const {
    return tipoEntidad;
}

void EntidadJuego::establecerTipo(TipoEntidad t) {
    tipoEntidad = t;
}

