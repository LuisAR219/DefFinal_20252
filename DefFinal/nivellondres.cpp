#include "NivelLondres.h"

NivelLondres::NivelLondres(QObject* parent)
    : Nivel(parent), soldadoJugador(nullptr), bombarderoEnemigo(nullptr), explosionesRecibidas(0)
{}

void NivelLondres::inicializar() {
    soldadoJugador = new Soldado();
    soldadoJugador->setTipoEntidad(JUGADOR);
    entidades.append(soldadoJugador);

    bombarderoEnemigo = new AvionEnemigo();
    bombarderoEnemigo->setTipoEntidad(ENEMIGO);
    entidades.append(bombarderoEnemigo);

    bombarderoEnemigo->inicializarMotorAprendizaje();
}

void NivelLondres::actualizar(float dt) {
    for (EntidadJuego* entidad : entidades) {
        entidad->actualizar(dt);
    }

    if (bombarderoEnemigo->verificaImpacto(soldadoJugador)) {
        explosionesRecibidas++;
        soldadoJugador->reaccionarAExplosión();
    }

    bombarderoEnemigo->adaptarEstrategia(soldadoJugador->getHistorialMovimiento());

    if (explosionesRecibidas >= maxExplosionesPermitidas) {
        completado = true;
    }
}

bool NivelLondres::nivelCompletado() const {
    return completado;
}
