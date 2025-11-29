#ifndef BARCO_H
#define BARCO_H

#include "entidadjuego.h"
#include <QPainter>

class Barco : public EntidadJuego
{
    Q_OBJECT

public:
    explicit Barco(QObject* parent = nullptr,
                   int tipo_ = 0,
                   const QVector2D& posInicial = QVector2D(900.0f, 700.0f));

    ~Barco() override {}

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    void recibirDanio(float cantidad);
    bool estaDestruido() const { return vida <= 0.0f; }

    int obtenerTipo() const { return tipo; }
    float obtenerVida() const { return vida; }

private:
    int tipo;
    float vida;
    float velocidadX;
    float anchoSprite;
    float altoSprite;
    bool destruido;

};

#endif // BARCO_H
