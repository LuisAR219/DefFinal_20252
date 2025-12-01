#include "entidadfija.h"
#include <QPainter>

EntidadFija::EntidadFija(QObject* parent,
                         const QVector2D& posicionInicial,
                         float radioColisionInicial,
                         int danoColision,
                         bool esDestructible)
    : EntidadJuego(parent,
                   posicionInicial,
                   1.0f,
                   radioColisionInicial,
                   NEUTRAL),
    danoAlColision(danoColision),
    esDestructibleFlag(esDestructible)
{
    velocidad = QVector2D(0.0f, 0.0f);
}

EntidadFija::~EntidadFija()
{
}

void EntidadFija::actualizar(float dt)
{
    Q_UNUSED(dt);
}

void EntidadFija::aplicarFuerza(const QVector2D& fuerza)
{
    Q_UNUSED(fuerza);
}

bool EntidadFija::colisionaCon(const EntidadJuego* otra) const
{
    float distancia = (otra->getPosicion() - posicion).length();
    float sumaRadios = radioColision + otra->getRadioColision();
    return distancia < sumaRadios;
}

void EntidadFija::pintar(QPainter* pintor)
{
    pintor->setBrush(Qt::darkYellow);
    pintor->drawEllipse(QPointF(posicion.x(), posicion.y()),
                        radioColision,
                        radioColision);
}

int EntidadFija::obtenerDano() const
{
    return danoAlColision;
}

bool EntidadFija::obtenerEsDestructible() const
{
    return esDestructibleFlag;
}
