#ifndef NIVELLONDRES_H
#define NIVELLONDRES_H

#include "Nivel.h"
#include "Soldado.h"
#include "AvionEnemigo.h"

class NivelLondres : public Nivel {
    Q_OBJECT

public:
    NivelLondres();

private:
    Soldado* soldadoJugador;
    AvionEnemigo* bombarderoEnemigo;
    int explosionesRecibidas;
    const int maxExplosionesPermitidas = 3;
};

#endif // NIVELLONDRES_H

