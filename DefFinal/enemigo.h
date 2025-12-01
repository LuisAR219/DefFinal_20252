#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "entidadjuego.h"
#include <QPainter>

class Enemigo : public EntidadJuego {
    Q_OBJECT

public:
    Enemigo(QObject* parent = nullptr,
            const QVector2D& posicionInicial = QVector2D(0.0f, 0.0f),
            float velocidadPatrulla = 40.0f,
            int danoChoque = 15,
            float radio = 18.0f);

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override { Q_UNUSED(fuerza); }
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    int obtenerDano() const { return dano; }

signals:
    void disparoGenerado(EntidadJuego* proyectil);

private:
    float velocidadPatrulla;
    int dano;
    float origenX;
    float rangoPatrulla;
    int direccion;
    float tiempoDesdeUltimoDisparo;
    float tiempoProximoDisparo;

    float generarProximoDisparo();
};

#endif
