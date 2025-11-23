#include "enemigo.h"
#include <QtMath>

Enemigo::Enemigo(QObject* parent,
                 const QVector2D& posicionInicial,
                 float velocidadY,
                 int danoChoque,
                 float radio)
    : EntidadJuego(parent, posicionInicial, 1.0f, radio, ENEMIGO),
    velocidadVertical(velocidadY),
    dano(danoChoque)
{
}

void Enemigo::actualizar(float dt)
{
    // movimiento simple hacia abajo
    posicion.setY(posicion.y() + velocidadVertical * dt);

    if (posicion.y() > 800.0f) {
        emit solicitudEliminar(this);
    }
}

void Enemigo::aplicarFuerza(const QVector2D&)
{
    // Enemigos simples no usan fuerzas por ahora
}

bool Enemigo::colisionaCon(const EntidadJuego* otra) const
{
    float dist = (otra->obtenerPosicion() - posicion).length();
    return dist < (radioColision + otra->obtenerRadioColision());
}

int Enemigo::obtenerDano() const
{
    return dano;
}

void Enemigo::pintar(QPainter* pintor)
{
    pintor->save();
    pintor->setPen(Qt::black);
    pintor->setBrush(Qt::red);

    float d = radioColision * 2.0f;
    pintor->drawEllipse(QRectF(posicion.x() - radioColision,
                               posicion.y() - radioColision,
                               d, d));

    pintor->restore();
}
