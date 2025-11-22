#ifndef ENTIDADFIJA_H
#define ENTIDADFIJA_H

#include "entidadjuego.h"

class EntidadFija : public EntidadJuego
{
    Q_OBJECT

public:
    explicit EntidadFija(QObject* parent = nullptr,
                         const QVector2D& posicionInicial = QVector2D(0.0f, 0.0f),
                         float radioColisionInicial = 12.0f,
                         int danoColision = 20,
                         bool esDestructible = false);

    virtual ~EntidadFija();
    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    // Getters
    int obtenerDano() const;
    bool obtenerEsDestructible() const;

private:
    int danoAlColision;
    bool esDestructibleFlag;
};

#endif // ENTIDADFIJA_H

