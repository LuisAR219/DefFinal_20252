#include "obstaculo.h"
#include <QPainter>

Obstaculo::Obstaculo(Tipo tipo_,
                     QObject* parent,
                     const QVector2D& posInicial)
    : EntidadJuego(parent, posInicial, 1.0f, 15.0f, NEUTRAL),
    tipo(tipo_)
{
    switch (tipo)
    {
    case RAPIDO:
        velocidad = 240.0f;
        dano = 10;
        sprite = QPixmap(":/imagenes/rueda.png");
        anchoSprite = 26;
        altoSprite  = 26;
        break;

    case MEDIO:
        velocidad = 180.0f;
        dano = 20;
        sprite = QPixmap(":/imagenes/barril.png");
        anchoSprite = 34;
        altoSprite  = 34;
        break;

    case LENTO:
        velocidad = 120.0f;
        dano = 30;
        sprite = QPixmap(":/imagenes/mina.png");
        anchoSprite = 48;
        altoSprite  = 48;
        break;
    }
}

void Obstaculo::actualizar(float dt)
{
    posicion.setY(posicion.y() + velocidad * dt);

    if (posicion.y() > 900.0f)
        emit solicitudEliminar(this);
}

bool Obstaculo::colisionaCon(const EntidadJuego* otra) const
{
    float d = (otra->obtenerPosicion() - posicion).length();
    return d < (radioColision + otra->obtenerRadioColision());
}

void Obstaculo::pintar(QPainter* p)
{
    QRectF rect(posicion.x() - anchoSprite/2,
                posicion.y() - altoSprite/2,
                1.5*anchoSprite,
                1.5*altoSprite);

    p->drawPixmap(rect.toRect(), sprite);

}
