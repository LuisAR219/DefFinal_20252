#include "proyectil.h"
#include <QtMath>

Proyectil::Proyectil(QObject* parent,
                     const QVector2D& posicionInicial,
                     const QVector2D& direccion,
                     float velocidad_,
                     int dano,
                     float radio)
    : EntidadJuego(parent, posicionInicial, 1.0f, radio, NEUTRAL),
    dirNormalizada(direccion),
    velocidad(velocidad_),
    danoImpacto(dano)
{
    // Normalizar dirección (si es cero, indicar hacia abajo)
    if (qFuzzyIsNull(dirNormalizada.length())) {
        dirNormalizada = QVector2D(0.0f, 1.0f);
    } else {
        dirNormalizada /= dirNormalizada.length();
    }
}

void Proyectil::actualizar(float dt)
{
    // Movimiento rectilíneo a velocidad constante
    posicion += dirNormalizada * velocidad * dt;

    // Si sale de pantalla (umbral), solicitar eliminación
    const float limiteAbajo = 900.0f;
    const float limiteArriba = -100.0f;
    if (posicion.y() > limiteAbajo || posicion.y() < limiteArriba) {
        emit solicitudEliminar(this);
    }
}

void Proyectil::aplicarFuerza(const QVector2D& )
{
    // No usado para proyectiles simples
}

bool Proyectil::colisionaCon(const EntidadJuego* otra) const
{
    float dist = (otra->obtenerPosicion() - posicion).length();
    return dist < (radioColision + otra->obtenerRadioColision());
}

void Proyectil::pintar(QPainter* pintor)
{
    pintor->save();
    pintor->setPen(Qt::NoPen);
    pintor->setBrush(Qt::yellow);
    float d = radioColision * 2.0f;
    pintor->drawEllipse(QRectF(posicion.x() - radioColision,
                               posicion.y() - radioColision,
                               d, d));
    pintor->restore();
}
