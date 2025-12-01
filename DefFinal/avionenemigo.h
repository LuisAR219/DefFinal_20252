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
    bool verificaImpacto(const EntidadJuego* jugador) const;
    void pintar(QPainter* pintor) override; // AÑADIDO

    void inicializarMotorAprendizaje();
    void adaptarEstrategia(const QQueue<QVector2D>& historialJugador);
    Proyectil* soltarBomba();

    bool getDireccionDerecha() const { return bombarderoDerecha; }
    void setDireccionDerecha(bool dir) { bombarderoDerecha = dir; }

private:
    float frecuenciaLanzamiento;
    float tiempoDesdeUltimoDisparo;
    MotorAprendizaje* motorIA;
    bool bombarderoDerecha;
};

#endif
