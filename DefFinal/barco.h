#ifndef BARCO_H
#define BARCO_H

#include "entidadjuego.h"
#include <QPainter>

class Barco : public EntidadJuego {
    Q_OBJECT

public:
    explicit Barco(QObject* parent = nullptr,
                   int tipo_ = 0,
                   const QVector2D& posInicial = QVector2D(900.0f, 700.0f));

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override { Q_UNUSED(fuerza); }
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    void recibirDanio(float cantidad);
    bool estaDestruido() const { return vida <= 0.0f; }
    int obtenerTipo() const { return tipo; }

    void setAnchoHitbox(float w) { anchoHitbox = w; }
    void setAltoHitbox(float h) { altoHitbox = h; }
    float getAnchoHitbox() const { return anchoHitbox; }
    float getAltoHitbox() const { return altoHitbox; }

signals:
    void solicitudEliminar(EntidadJuego* entidad);

private:
    int tipo;
    float velocidadX;
    float anchoSprite;
    float altoSprite;
    bool destruido;

    float anchoHitbox = 240.0f;
    float altoHitbox = 120.0f;
};

#endif
