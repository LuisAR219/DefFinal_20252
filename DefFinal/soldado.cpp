#include "Soldado.h"

Soldado::Soldado(QObject* parent)
    : EntidadJuego(parent),
    factorInercia(0.85f),
    direccionActual(0, 0),
    velocidadMax(180.0f),
    invulnerableTimer(0.0f)
{
    setTipoEntidad(JUGADOR);
    setVida(100.0f);
}

void Soldado::aplicarFuerza(const QVector2D& fuerza) {
    velocidad += fuerza;
}

void Soldado::actualizar(float dt) {
    if (invulnerableTimer > 0.0f) {
        invulnerableTimer = qMax(0.0f, invulnerableTimer - dt);
    }

    QVector2D objetivo = direccionActual * velocidadMax;
    velocidad = velocidad * factorInercia + objetivo * (1.0f - factorInercia);

    posicion += velocidad * dt;

    registrarPosicion();
}

bool Soldado::colisionaCon(const EntidadJuego* otra) const {
    return (posicion - otra->getPosicion()).length() < 20.0f;
}

void Soldado::recibirInput(const QVector2D& direccion) {
    if (!qFuzzyIsNull(direccion.length())) {
        direccionActual = direccion.normalized();
    } else {
        direccionActual = QVector2D(0, 0);
    }
}

void Soldado::reaccionarAExplosión() {
    if (invulnerableTimer > 0.0f) return;

    invulnerableTimer = invulnerableDuration;

    setVida(qMax(0.0f, getVida() - 25.0f));
    velocidad += QVector2D(-direccionActual.x(), -direccionActual.y()) * 80.0f;
}

const QQueue<QVector2D>& Soldado::getHistorialMovimiento() const {
    return historialMovimiento;
}

void Soldado::registrarPosicion() {
    historialMovimiento.enqueue(posicion);
    const int maxMuestras = 60;
    while (historialMovimiento.size() > maxMuestras) {
        historialMovimiento.dequeue();
    }
}
