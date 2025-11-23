#include "Nivel.h"

Nivel::Nivel(QObject* parent)
    : QObject(parent), completado(false)
{
    connect(&temporizadorNivel, &QTimer::timeout, this, &Nivel::onUpdate);
    temporizadorNivel.start(16);
}

void Nivel::onUpdate() {
    actualizar(0.016f);
}
