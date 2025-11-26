#include "AvionEnemigo.h"

AvionEnemigo::AvionEnemigo(QObject* parent)
    : EntidadMovil(parent), frecuenciaLanzamiento(1.5f), tiempoDesdeUltimoDisparo(0.0f), motorIA(nullptr)
{
    setMasa(0.1f);
}

void AvionEnemigo::actualizar(float dt) {
    float velocidadX = 100.0f;

    if (bombarderoDerecha) {
        posicion.setX(posicion.x() + velocidadX * dt);
        if (posicion.x() > 750) {
            bombarderoDerecha = false;
        }
    } else {
        posicion.setX(posicion.x() - velocidadX * dt);
        if (posicion.x() < 50) {
            bombarderoDerecha = true;
        }
    }

}


bool AvionEnemigo::verificaImpacto(const EntidadJuego* jugador) const {
    return (posicion - jugador->getPosicion()).length() < 50.0f;
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
        Proyectil* bomba = new Proyectil(nullptr);
        bomba->setPosicion(this->getPosicion());
        bomba->lanzar(QVector2D(0,1), 150.0f);
        bomba->setTipoEntidad(ENEMIGO);
        return bomba;
    }
    return nullptr;
}
