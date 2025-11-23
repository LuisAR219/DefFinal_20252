#ifndef CONTROLSPAWN_H
#define CONTROLSPAWN_H

#include <QObject>
#include <QVector>
#include "spawnevent.h"
#include "enemigo.h"

class ControlSpawn : public QObject
{
    Q_OBJECT

public:
    explicit ControlSpawn(QObject* parent = nullptr);

    void reiniciar();
    void actualizar(float dt);
    void agregarEvento(const SpawnEvent& evento);

signals:
    void enemigoGenerado(Enemigo* nuevo);

private:
    QVector<SpawnEvent> eventos;  // Lista de eventos programados
    float tiempoTranscurrido;     // Tiempo acumulado
    int indiceEventoActual;       // Índice del siguiente evento a generar
};

#endif // CONTROLSPAWN_H



