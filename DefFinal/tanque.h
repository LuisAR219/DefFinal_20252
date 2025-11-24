#ifndef TANQUE_H
#define TANQUE_H

#include "EntidadMovil.h"
#include "Proyectil.h"

class Tanque : public EntidadMovil {
    Q_OBJECT

public:
    explicit Tanque(QObject* parent = nullptr);

    void actualizar(float dt) override;
    bool colisionaCon(const EntidadJuego* otra) const override;

    void atacar(const QVector2D& direccion, float potencia);

signals:
    void proyectilCreado(Proyectil* nuevo);

private:
    float velocidadObjetivo;
    float coeficienteFriccion;
};

#endif // TANQUE_H
