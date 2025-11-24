#include "AvionEnemigo.h"

AvionEnemigo::AvionEnemigo(QObject* parent)
    : EntidadMovil(parent), frecuenciaLanzamiento(2.0f), tiempoDesdeUltimoDisparo(0.0f), motorIA(nullptr)
{}

void AvionEnemigo::actualizar(float dt) {
    aplicarFisica(dt);
    tiempoDesdeUltimoDisparo += dt;
}

bool AvionEnemigo::colisionaCon(const EntidadJuego* otra) const {
    return (posicion - otra->getPosicion()).length() < 40.0f;
}

void AvionEnemigo::inicializarMotorAprendizaje() {
    motorIA = new MotorAprendizaje();
}

void AvionEnemigo::adaptarEstrategia(const QQueue<QVector2D>& historialJugador) {
    if(motorIA) motorIA->analizarMovimiento(historialJugador);
}

Proyectil* AvionEnemigo::soltarBomba() {
    if(tiempoDesdeUltimoDisparo >= frecuenciaLanzamiento) {
        tiempoDesdeUltimoDisparo = 0.0f;
        Proyectil* bomba = new Proyectil();
        bomba->setPosicion(this->getPosicion());
        bomba->lanzar(QVector2D(0,1), 150.0f);
        bomba->setTipoEntidad(ENEMIGO);
        return bomba;
    }
    return nullptr;
}
