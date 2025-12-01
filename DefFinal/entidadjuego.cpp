#include "EntidadJuego.h"

EntidadJuego::EntidadJuego(QObject* parent,
                           const QVector2D& posInicial,
                           float masaInicial,
                           float radioColisionInicial,
                           TipoEntidad tipoInicial)
    : QObject(parent),
    posicion(posInicial),
    velocidad(0, 0),
    vida(100.0f),
    tipo(tipoInicial),
    masa(masaInicial),
    radioColision(radioColisionInicial)
{}

