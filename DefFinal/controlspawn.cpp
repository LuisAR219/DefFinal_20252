#include "controlspawn.h"

ControlSpawn::ControlSpawn(QObject* parent)
    : QObject(parent),
    tiempoTranscurrido(0.0f),
    indiceEventoActual(0)
{
}

void ControlSpawn::reiniciar()
{
    tiempoTranscurrido = 0.0f;
    indiceEventoActual = 0;
}

void ControlSpawn::agregarEvento(const SpawnEvent& evento)
{
    eventos.append(evento);
}

void ControlSpawn::actualizar(float dt)
{
    if (indiceEventoActual >= eventos.size())
        return;

    tiempoTranscurrido += dt;

    // Generar todos los eventos cuyo tiempo ya llegó
    while (indiceEventoActual < eventos.size() &&
           tiempoTranscurrido >= eventos[indiceEventoActual].tiempo)
    {
        const SpawnEvent& ev = eventos[indiceEventoActual];

        Enemigo* nuevo = new Enemigo(
            nullptr,        // sin parent -> dueño será Nivel1
            ev.posicion,
            ev.velocidad,
            ev.dano,
            ev.radio
            );

        emit enemigoGenerado(nuevo);

        indiceEventoActual++;
    }
}

void ControlSpawn::generarEnemigoEn(const QVector2D& posicion)
{
    Enemigo* nuevo = new Enemigo(nullptr, posicion, 40.0f, 15, 18.0f);
    emit enemigoGenerado(nuevo);
}



