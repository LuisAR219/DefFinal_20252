#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "entidadjuego.h"
#include <QPainter>

class Enemigo : public EntidadJuego
{
    Q_OBJECT

public:
    Enemigo(QObject* parent = nullptr,
            const QVector2D& posicionInicial = QVector2D(0.0f, 0.0f),
            float velocidadPatrulla = 40.0f,
            int danoChoque = 15,
            float radio = 18.0f);

    ~Enemigo() override {}

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    int obtenerDano() const;

signals:
    // Señal para notificar que el enemigo generó un proyectil
    void disparoGenerado(EntidadJuego* proyectil);

private:
    // Patrulla horizontal
    float velocidadPatrulla;
    int dano;

    float origenX;        // posición x de referencia
    float rangoPatrulla;  // +/- desde origenX
    int direccion;        // +1 o -1

    // Disparo
    float tiempoDesdeUltimoDisparo;
    float tiempoProximoDisparo; // tiempo objetivo para próximo disparo

    float generarProximoDisparo(); // devuelve valor aleatorio entre 1.0 y 2.5
};
#endif // ENEMIGO_H



