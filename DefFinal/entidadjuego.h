#ifndef ENTIDADJUEGO_H
#define ENTIDADJUEGO_H

#include <QObject>
#include <QVector2D>
#include <QPainter>

enum TipoEntidad {
    JUGADOR,
    ENEMIGO,
    NEUTRAL
};

class EntidadJuego : public QObject {
    Q_OBJECT

public:
    explicit EntidadJuego(QObject* parent = nullptr,
                          const QVector2D& posInicial = QVector2D(0,0),
                          float masaInicial = 1.0f,
                          float radioColisionInicial = 30.0f,
                          TipoEntidad tipoInicial = NEUTRAL);

    virtual void actualizar(float dt) = 0;
    virtual void aplicarFuerza(const QVector2D& fuerza) = 0;
    virtual bool colisionaCon(const EntidadJuego* otra) const = 0;
    virtual void pintar(QPainter* pintor) = 0; // Método PURE VIRTUAL

    // Getters y setters
    QVector2D getPosicion() const { return posicion; }
    void setPosicion(const QVector2D& pos) { posicion = pos; }

    QVector2D getVelocidad() const { return velocidad; }
    void setVelocidad(const QVector2D& vel) { velocidad = vel; }

    float getVida() const { return vida; }
    void setVida(float v) { vida = v; }

    TipoEntidad getTipoEntidad() const { return tipo; }
    void setTipoEntidad(TipoEntidad t) { tipo = t; }

    float getMasa() const { return masa; }
    void setMasa(float m) { masa = m; }

    float getRadioColision() const { return radioColision; }
    void setRadioColision(float r) { radioColision = r; }

protected:
    QVector2D posicion;
    QVector2D velocidad;
    float vida;
    TipoEntidad tipo;
    float masa;
    float radioColision;
};

#endif
