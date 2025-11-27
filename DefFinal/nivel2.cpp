#include "nivel2.h"
#include <QDebug>

Nivel2::Nivel2(QObject* parent)
    : Nivel(parent),
    canon(nullptr),
    barraPotencia(nullptr),
    proyectilActual(nullptr),
    estadoActual(Fase_MovVertical),
    tiempo(0.0f),
    totalBarcosPorDestruir(0),
    barcosRestantes(0)
{
}

Nivel2::~Nivel2()
{
    for (EntidadJuego* e : entidades)
        delete e;
    entidades.clear();
}

void Nivel2::inicializar()
{
    qDebug() << "Inicializando Nivel 2 – Batalla de Pearl Harbor";

    tiempo = 0.0f;
    estadoActual = Fase_MovVertical;

    entidades.clear();
    barcosActivos.clear();
    proyectilActual = nullptr;

    // Crear cañón
    canon = new CanonJugador(nullptr);
    entidades.append(canon);

    // Crear barra de potencia
    barraPotencia = new BarraPotencia(nullptr);
    // NO se agrega aún a entidades porque solo se muestra en fase específica

    // Determinar cuántos barcos se generarán
    totalBarcosPorDestruir = 3 + (qrand() % 4);  // 3–6
    barcosRestantes = totalBarcosPorDestruir;

    // Inicialmente solo aparecerán máximo 2
}

void Nivel2::actualizar(float dt)
{
    tiempo += dt;

    gestionarFases(dt);
    gestionarBarcos(dt);
    verificarColisiones();

    // Actualizar entidades
    for (EntidadJuego* e : entidades)
        e->actualizar(dt);
}

bool Nivel2::nivelCompletado() const
{
    return barcosRestantes <= 0;
}

void Nivel2::gestionarFases(float dt)
{
    // Implementar después
}

void Nivel2::gestionarBarcos(float dt)
{
    // Implementar después
}

void Nivel2::verificarColisiones()
{
    // Implementar después
}
