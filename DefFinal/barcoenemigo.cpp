#include "barcoenemigo.h"
#include <QPainter>

BarcoEnemigo::BarcoEnemigo(QObject* parent, Tipo tipo)
    : EntidadJuego(parent, QVector2D(900.0f, 400.0f), 80, 30),
    tipoBarco(tipo)
{
    switch(tipo) {
    case Pequeno: velocidad = 60; vida = 40; break;
    case Mediano: velocidad = 40; vida = 80; break;
    case Grande:  velocidad = 20; vida = 140; break;
    }
}

void BarcoEnemigo::actualizar(float dt)
{
    posicion.setX(posicion.x() - velocidad * dt);
}

void BarcoEnemigo::pintar(QPainter* painter)
{
    painter->setBrush(Qt::blue);
    painter->drawRect(hitbox());
}

QRectF BarcoEnemigo::hitbox() const
{
    return QRectF(posicion.x(), posicion.y(), anchoSprite, altoSprite);
}

void BarcoEnemigo::recibirDano(float d)
{
    vida -= d;
}
