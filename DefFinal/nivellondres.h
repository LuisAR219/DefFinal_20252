#ifndef NIVELLONDRES_H
#define NIVELLONDRES_H

#include "Nivel.h"
#include "Soldado.h"
#include "AvionEnemigo.h"
#include <QMap>
#include <QPixmap>

class NivelLondres : public Nivel {
    Q_OBJECT

public:
    explicit NivelLondres(QObject* parent = nullptr);
    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;
    EntidadJuego* getJugador() const override { return soldadoJugador; }
    void setSpriteCache(const QMap<QString, QPixmap>& cache) override { spriteCache = cache; }
    int getExplosionesRecibidas() const;
    void resetearExplosiones();

private:
    Soldado* soldadoJugador;
    float tiempoDisparo;
    AvionEnemigo* bombarderoEnemigo;
    int explosionesRecibidas;
    const int maxExplosionesPermitidas = 3;
    QMap<QString, QPixmap> spriteCache;
};

#endif
