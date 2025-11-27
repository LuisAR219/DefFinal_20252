#ifndef PROYECTILCANON_H
#define PROYECTILCANON_H

#include "entidadjuego.h"

class ProyectilCanon : public EntidadJuego
{
public:
    ProyectilCanon(QObject* parent,
                   const QVector2D& posInicial,
                   float anguloGrados,
                   float velocidadInicial);

    void actualizar(float dt) override;
    void pintar(QPainter* painter) override;
    QRectF hitbox() const override;

    bool exploto() const { return explosionFinalizada; }
    bool impacto() const { return haImpactado; }

private:
    float vx, vy;
    float tiempoVuelo;
    bool haImpactado;
    bool explosionFinalizada;
};

#endif

