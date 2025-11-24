#ifndef AVIONENEMIGO_H
#define AVIONENEMIGO_H

#include "EntidadMovil.h"
#include "MotorAprendizaje.h"
#include "Proyectil.h"

class AvionEnemigo : public EntidadMovil {
    Q_OBJECT

public:
    explicit AvionEnemigo(QObject* parent = nullptr);

    void actualizar(float dt) override;
    bool colisionaCon(const EntidadJuego* otra) const override;

    void inicializarMotorAprendizaje();
    void adaptarEstrategia(const QQueue<QVector2D>& historialJugador);
    Proyectil* soltarBomba();

private:
    float frecuenciaLanzamiento;
    float tiempoDesdeUltimoDisparo;
    MotorAprendizaje* motorIA;
};

#endif // AVIONENEMIGO_H
