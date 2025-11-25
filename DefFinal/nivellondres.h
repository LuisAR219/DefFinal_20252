#ifndef NIVELLONDRES_H
#define NIVELLONDRES_H

#include "Nivel.h"
#include "Soldado.h"
#include "AvionEnemigo.h"

class NivelLondres : public Nivel {
    Q_OBJECT

public:
    explicit NivelLondres(QObject* parent = nullptr);
    Soldado* getJugador() const;

    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;

private:
    Soldado* soldadoJugador;
    AvionEnemigo* bombarderoEnemigo;
    int explosionesRecibidas;
    const int maxExplosionesPermitidas = 3;
};

#endif // NIVELLONDRES_H

