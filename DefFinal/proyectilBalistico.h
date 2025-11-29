#ifndef PROYECTILBALISTICO_H
#define PROYECTILBALISTICO_H

#include "entidadjuego.h"
#include <QVector2D>

class ProyectilBalistico : public EntidadJuego
{
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

    bool colisionaCon(const EntidadJuego* otra) const;
    void aplicarFuerza(const QVector2D& fuerza);

    // === NUEVOS MÉTODOS ===
    bool estaActivo() const { return activo; }
    float obtenerRadioExplosivo() const { return radioExpl; }
    int obtenerDanoBase() const { return danoBase; }

signals:
    void solicitudEliminar(EntidadJuego* e);

private:
    QVector2D velocidad;
    float gravedad;
    int danoBase;
    float radioExpl;
    bool activo = true;
};

#endif // PROYECTILBALISTICO_H
