#include "proyectilcanon.h"
#include <QPainter>
#include <QtMath>

ProyectilCanon::ProyectilCanon(QObject* parent,
                               const QVector2D& posInicial,
                               float anguloGrados,
                               float velocidadInicial)
    : EntidadJuego(parent, posInicial, 10, 10),
    tiempoVuelo(0.0f),
    haImpactado(false),
    explosionFinalizada(false)
{
    float rad = qDegreesToRadians(anguloGrados);
    vx = velocidadInicial * qCos(rad);
    vy = -velocidadInicial * qSin(rad);
}

void ProyectilCanon::actualizar(float dt)
{
}

void ProyectilCanon::pintar(QPainter* painter)
{
}

QRectF ProyectilCanon::hitbox() const
{
    return QRectF(posicion.x(), posicion.y(), anchoSprite, altoSprite);
}
