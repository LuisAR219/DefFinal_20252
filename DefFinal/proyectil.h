#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "EntidadMovil.h"

class Proyectil : public EntidadMovil {
    Q_OBJECT

public:
    explicit Proyectil(QObject* parent = nullptr,
                       const QVector2D& posInicial = QVector2D(0,0),
                       const QVector2D& velInicial = QVector2D(0,0),
                       float potencia = 150.0f,
                       float danoInicial = 25.0f,
                       float radioColisionInicial = 30.0f);

    void actualizar(float dt) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    void lanzar(const QVector2D& direccion, float potencia);
    float getDaño() const { return daño; }
    void setDaño(float d) { daño = d; }

private:
    float daño;
};

#endif
