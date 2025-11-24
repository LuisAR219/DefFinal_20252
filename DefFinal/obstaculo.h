#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "entidadjuego.h"
#include <QPixmap>

class Obstaculo : public EntidadJuego
{
    Q_OBJECT

public:
    enum Tipo { RAPIDO, MEDIO, LENTO };

    Obstaculo(Tipo tipo,
              QObject* parent = nullptr,
              const QVector2D& posInicial = QVector2D(0,0));

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D&) override {}
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    int obtenerDano() const { return dano; }

private:
    Tipo tipo;
    float velocidad;
    int dano;

    QPixmap sprite;
    int anchoSprite;
    int altoSprite;
};

#endif // OBSTACULO_H
