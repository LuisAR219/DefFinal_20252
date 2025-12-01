#ifndef SOLDADO_H
#define SOLDADO_H

#include "EntidadJuego.h"
#include <QQueue>
#include <QVector2D>

class Soldado : public EntidadJuego {
    Q_OBJECT

public:
    explicit Soldado(QObject* parent = nullptr);

    void actualizar(float dt) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void aplicarFuerza(const QVector2D& fuerza) override;

    void recibirInput(const QVector2D& direccion);
    void reaccionarAExplosión();
    const QQueue<QVector2D>& getHistorialMovimiento() const;

private:
    float factorInercia;
    QQueue<QVector2D> historialMovimiento;
    QVector2D direccionActual;
    float velocidadMax;
    float invulnerableTimer;
    const float invulnerableDuration = 0.5f;

    void registrarPosicion();
};

#endif // SOLDADO_H
