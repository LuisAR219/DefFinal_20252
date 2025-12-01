#ifndef PROYECTILBALISTICO_H
#define PROYECTILBALISTICO_H

#include "entidadjuego.h"
#include <QVector2D>
#include <QPainter>

class ProyectilBalistico : public EntidadJuego {
    Q_OBJECT

public:
    ProyectilBalistico(QObject* parent,
                       const QVector2D& posInicial,
                       float velocidadInicial,
                       float anguloRad,
                       float gravedad_,
                       int danoBase_,
                       float radioExpl_,
                       float radioCol);

    void actualizar(float dt) override;
    void pintar(QPainter* pintor) override;
    void aplicarFuerza(const QVector2D& fuerza) override;

    bool colisionaCon(const EntidadJuego* otra) const override;

    bool estaActivo() const { return activo; }
    float obtenerRadioExplosivo() const { return radioExpl; }
    int obtenerDanoBase() const { return danoBase; }
    float getPosicionX() const { return getPosicion().x(); }
    float getPosicionY() const { return getPosicion().y(); }

signals:
    void solicitudEliminar(EntidadJuego* e);

private:
    // CORRECCIÓN: Orden alfabético/declaración
    bool activo = true;
    int danoBase;
    float gravedad;
    float radioExpl;
    QVector2D velocidad;
};

#endif
