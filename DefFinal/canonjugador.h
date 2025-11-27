#ifndef CANONJUGADOR_H
#define CANONJUGADOR_H

#include "entidadjuego.h"

class CanonJugador : public EntidadJuego
{
public:
    CanonJugador(QObject* parent = nullptr);
    ~CanonJugador() override = default;

    void actualizar(float dt) override;
    void pintar(QPainter* painter) override;
    QRectF hitbox() const override;

    // Movimiento vertical
    void fijarAltura();
    float obtenerAltura() const { return posicion.y(); }

    // Ángulo
    void iniciarOscilacionAngulo();
    void fijarAngulo();
    float obtenerAngulo() const { return angulo; }

    // Potencia
    void fijarPotencia(float p);
    float obtenerPotencia() const { return potencia; }

    // Proyectil
    bool listoParaDisparar() const { return tieneAnguloFijo && tienePotenciaFija; }
    void reiniciarEstado();

private:
    float angulo;
    float potencia;
    float tiempoLocal;

    bool alturaFija;
    bool anguloOscilando;
    bool tieneAnguloFijo;
    bool tienePotenciaFija;

    float y_base;
    float amplitud;
    float omega;
};

#endif

