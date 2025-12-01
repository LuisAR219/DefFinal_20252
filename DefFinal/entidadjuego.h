#ifndef ENTIDADJUEGO_H
#define ENTIDADJUEGO_H

#include <QObject>
#include <QVector2D>

enum TipoEntidad {
    JUGADOR,
    ENEMIGO,
    NEUTRAL
};

class EntidadJuego : public QObject {
    Q_OBJECT

public:
    explicit EntidadJuego(QObject* parent = nullptr);

    virtual void actualizar(float dt) = 0;
    virtual void aplicarFuerza(const QVector2D& fuerza) = 0;
    virtual bool colisionaCon(const EntidadJuego* otra) const = 0;

    QVector2D getPosicion() const;
    void setPosicion(const QVector2D& pos);

    QVector2D getVelocidad() const;
    void setVelocidad(const QVector2D& vel);

    float getVida() const;
    void setVida(float v);

    TipoEntidad getTipoEntidad() const;
    void setTipoEntidad(TipoEntidad tipo);

protected:
    QVector2D posicion;
    QVector2D velocidad;
    float vida;
    TipoEntidad tipo;
};

#endif // ENTIDADJUEGO_H
