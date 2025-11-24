#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "entidadjuego.h"
#include <QPainter>

class Proyectil : public EntidadJuego
{
    Q_OBJECT

public:
    // direccion: (0,1) hacia abajo, velocidad en px/s, daño int
    explicit Proyectil(QObject* parent = nullptr,
                       const QVector2D& posicionInicial = QVector2D(0.0f, 0.0f),
                       const QVector2D& direccion = QVector2D(0.0f, 1.0f),
                       float velocidad = 260.0f,
                       int dano = 10,
                       float radio = 5.0f);

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    int obtenerDano() const { return danoImpacto; }

private:
    QVector2D dirNormalizada;
    float velocidad;
    int danoImpacto;
};
#endif // PROYECTIL_H

