#ifndef BARRA_POTENCIA_H
#define BARRA_POTENCIA_H

#include "entidadjuego.h"

class BarraPotencia : public EntidadJuego
{
public:
    BarraPotencia(QObject* parent);

    void actualizar(float dt) override;
    void pintar(QPainter* painter) override;
    QRectF hitbox() const override;

    float obtenerPotencia() const;

    void iniciarOscilacion();
    void detenerOscilacion();

private:
    float indicador;
    bool subiendo;
    bool oscilando;
};

#endif

