#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "EntidadMovil.h"

class Proyectil : public EntidadMovil {
    Q_OBJECT

public:
    explicit Proyectil(QObject* parent = nullptr);

    void actualizar(float dt) override;
    bool colisionaCon(const EntidadJuego* otra) const override;

    void lanzar(const QVector2D& direccion, float potencia);
    float getDaño() const;
    void setDaño(float d);

private:
    float daño;
};

#endif // PROYECTIL_H
