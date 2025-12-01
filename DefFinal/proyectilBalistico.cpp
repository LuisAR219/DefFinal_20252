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
    // CORRECCIÓN: Orden de inicialización igual al de declaración
    activo(true),
    danoBase(danoBase_),
    gravedad(gravedad_),
    radioExpl(radioExpl_),
    velocidad(QVector2D(velocidadInicial * qCos(anguloRad), -velocidadInicial * qSin(anguloRad)))
{
}

void ProyectilBalistico::actualizar(float dt) {
    if (!activo) return;

    velocidad.setY(velocidad.y() + gravedad * dt);
    setPosicion(getPosicion() + velocidad * dt);

    const float limiteAbajo = 1200.0f;
    const float limiteDerecha = 2000.0f;
    const float limiteIzquierda = -200.0f;

    if (getPosicion().y() > limiteAbajo ||
        getPosicion().x() > limiteDerecha ||
        getPosicion().x() < limiteIzquierda) {
        activo = false;
        emit solicitudEliminar(this);
    }
}

void ProyectilBalistico::aplicarFuerza(const QVector2D& fuerza) {
    velocidad += fuerza / getMasa();
}

bool ProyectilBalistico::colisionaCon(const EntidadJuego* otra) const {
    float dist = (otra->getPosicion() - getPosicion()).length();
    return dist < (getRadioColision() + otra->getRadioColision());
}

void ProyectilBalistico::pintar(QPainter* pintor) {
    if (!activo) return;

    pintor->setBrush(Qt::black);
    pintor->setPen(Qt::NoPen);
    pintor->drawEllipse(QPointF(getPosicion().x(), getPosicion().y()),
                        getRadioColision(), getRadioColision());
}
