#ifndef SPAWNEVENT_H
#define SPAWNEVENT_H

#include <QVector2D>

struct SpawnEvent
{
    float tiempo;
    QVector2D posicion;
    float velocidad;
    int dano;
    float radio;
};

#endif // SPAWNEVENT_H


