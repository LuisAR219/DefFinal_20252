#include "canonjugador.h"
#include <QPainter>
#include <QtMath>

CanonJugador::CanonJugador(QObject* parent)
    : EntidadJuego(parent,
                   QVector2D(50.0f, 300.0f),
                   40, 20),
    angulo(0.0f),
    potencia(0.0f),
    tiempoLocal(0.0f),
    alturaFija(false),
    anguloOscilando(false),
    tieneAnguloFijo(false),
    tienePotenciaFija(false),
    y_base(300.0f),
    amplitud(150.0f),
    omega(2.0f)
{
}

void CanonJugador::actualizar(float dt)
{
}

void CanonJugador::pintar(QPainter* painter)
{
}

QRectF CanonJugador::hitbox() const
{
    return QRectF(posicion.x(), posicion.y(), anchoSprite, altoSprite);
}

void CanonJugador::fijarAltura()
{
    alturaFija = true;
}

void CanonJugador::iniciarOscilacionAngulo()
{
    anguloOscilando = true;
}

void CanonJugador::fijarAngulo()
{
    anguloOscilando = false;
    tieneAnguloFijo = true;
}

void CanonJugador::fijarPotencia(float p)
{
    potencia = p;
    tienePotenciaFija = true;
}

void CanonJugador::reiniciarEstado()
{
    alturaFija = false;
    anguloOscilando = false;
    tieneAnguloFijo = false;
    tienePotenciaFija = false;
}
