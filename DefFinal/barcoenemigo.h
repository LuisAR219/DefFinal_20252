#ifndef BARCOENEMIGO_H
#define BARCOENEMIGO_H

#include "entidadjuego.h"

class BarcoEnemigo : public EntidadJuego
{
public:
    enum Tipo { Pequeno, Mediano, Grande };

    BarcoEnemigo(QObject* parent, Tipo tipo);

    void actualizar(float dt) override;
    void pintar(QPainter* painter) override;
    QRectF hitbox() const override;

    void recibirDano(float d);
    bool destruido() const { return vida <= 0; }

private:
    Tipo tipoBarco;
    float velocidad;
    float vida;
};

#endif

