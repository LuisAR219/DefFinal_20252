#include "proyectilbalistico.h"
#include <QPainter>
#include <QtMath>

ProyectilBalistico::ProyectilBalistico(QObject* parent,
                                       const QVector2D& posInicial,
                                       float velocidadInicial,
                                       float anguloRad,
                                       float gravedad_,
                                       int danoBase_,
                                       float radioExpl_,
                                       float radioCol)
    : EntidadJuego(parent, posInicial, 1.0f, radioCol, NEUTRAL),
    gravedad(gravedad_),
    danoBase(danoBase_),
    radioExpl(radioExpl_)
{
    float vx = velocidadInicial * qCos(anguloRad);
    float vy = -velocidadInicial * qSin(anguloRad);
    velocidad = QVector2D(vx, vy);
}

void ProyectilBalistico::actualizar(float dt)
{
    if (!activo) return;

    velocidad.setY(velocidad.y() + gravedad * dt);
    posicion += velocidad * dt;

    const float limiteAbajo = 1200.0f;
    const float limiteDerecha = 2000.0f;
    const float limiteIzquierda = -200.0f;

    if (posicion.y() > limiteAbajo ||
        posicion.x() > limiteDerecha ||
        posicion.x() < limiteIzquierda)
    {
        activo = false;
        emit solicitudEliminar(this);
    }
}

void ProyectilBalistico::aplicarFuerza(const QVector2D& fuerza)
{
    velocidad += fuerza / masa;
}

bool ProyectilBalistico::colisionaCon(const EntidadJuego* otra) const
{
    float dist = (otra->obtenerPosicion() - posicion).length();
    return dist < (radioColision + otra->obtenerRadioColision());
}

void ProyectilBalistico::pintar(QPainter* pintor)
{
    if (!activo) return;

    pintor->setBrush(Qt::black);
    pintor->setPen(Qt::NoPen);
    float r = radioColision;
    pintor->drawEllipse(QPointF(posicion.x(), posicion.y()), r, r);
}
