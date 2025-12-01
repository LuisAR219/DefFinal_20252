#include "Nivel.h"

Nivel::Nivel(QObject* parent)
    : QObject(parent), completado(false)
{
    connect(&temporizadorNivel, &QTimer::timeout, this, &Nivel::onUpdate);
    temporizadorNivel.start(16);
}
