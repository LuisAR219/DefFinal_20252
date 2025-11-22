#include "controltanque.h"

ControlTanque::ControlTanque(QObject* parent)
    : QObject(parent),
    acelerando(false),
    frenando(false),
    disparando(false),
    aceleracionMax(150.0f),
    coefFriccion(6.0f),
    factorSuavizado(0.18f),
    aceleracionActual(0.0f)
{
}

void ControlTanque::establecerInput(bool acelera, bool frena, bool dispara)
{
    acelerando = acelera;
    frenando   = frena;
    disparando = dispara;
}

void ControlTanque::establecerAceleracionMax(float valor)
{
    if (valor > 0.0f)
        aceleracionMax = valor;
}

void ControlTanque::establecerCoefFriccion(float valor)
{
    if (valor >= 0.0f)
        coefFriccion = valor;
}

void ControlTanque::establecerFactorSuavizado(float valor)
{
    if (valor >= 0.0f && valor <= 1.0f)
        factorSuavizado = valor;
}

QVector2D ControlTanque::calcularFuerza(const QVector2D& velocidadActual, float masa)
{
    float aceleracionDeseada = 0.0f;

    if (acelerando)
        aceleracionDeseada += aceleracionMax;

    if (frenando)
        aceleracionDeseada -= aceleracionMax;

    aceleracionActual =
        (1.0f - factorSuavizado) * aceleracionActual +
        factorSuavizado        * aceleracionDeseada;

    float friccion = -coefFriccion * velocidadActual.length();

    float aceleracionTotal = aceleracionActual + friccion;

    float fuerza = aceleracionTotal * masa;

    return QVector2D(0.0f, -fuerza);
}
