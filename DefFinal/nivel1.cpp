#include "nivel1.h"
#include "enemigo.h"
#include "entidadfija.h"

#include <QDebug>

Nivel1::Nivel1(QObject* parent)
    : Nivel(parent),
    jugador(nullptr),
    gestorSpawn(nullptr),
    tiempoNivel(0.0f),
    distanciaObjetivo(1800.0f)
{
}

Nivel1::~Nivel1()
{
    // Limpiar entidades del nivel
    for (EntidadJuego* e : entidades)
        delete e;
    entidades.clear();

    // gestorSpawn tiene parent = this; Qt lo liberará en cascada
}

void Nivel1::inicializar()
{
    qDebug() << "Inicializando Nivel 1 – Batalla de Kursk";

    tiempoNivel = 0.0f;
    completado = false;

    // Limpiar entidades previas
    for (EntidadJuego* e : entidades)
        delete e;
    entidades.clear();

    // --------------------------
    // 1. Crear al jugador
    // --------------------------
    jugador = new TanqueJugador(nullptr, QVector2D(400.0f, 520.0f));
    jugador->establecerLimites(80.0f, 720.0f, 20.0f, 560.0f);
    entidades.append(jugador);

    // --------------------------
    // 2. Crear el gestor de spawn
    // --------------------------
    gestorSpawn = new ControlSpawn(this);

    connect(gestorSpawn, &ControlSpawn::enemigoGenerado,
            this, &Nivel1::onEnemigoGenerado);

    gestorSpawn->reiniciar();

    // --------------------------
    // 3. Programar eventos fijos
    // --------------------------
    SpawnEvent ev;

    ev.tiempo = 1.0f;  ev.posicion = {100, 0}; ev.velocidad = 100; ev.dano = 10; ev.radio = 16;
    gestorSpawn->agregarEvento(ev);

    ev.tiempo = 2.0f;  ev.posicion = {300, 0}; ev.velocidad = 120; ev.dano = 12; ev.radio = 18;
    gestorSpawn->agregarEvento(ev);

    ev.tiempo = 3.0f;  ev.posicion = {500, 0}; ev.velocidad = 110; ev.dano = 12; ev.radio = 18;
    gestorSpawn->agregarEvento(ev);

    ev.tiempo = 4.0f;  ev.posicion = {200, 0}; ev.velocidad = 130; ev.dano = 15; ev.radio = 18;
    gestorSpawn->agregarEvento(ev);

    ev.tiempo = 5.0f;  ev.posicion = {400, 0}; ev.velocidad = 115; ev.dano = 10; ev.radio = 16;
    gestorSpawn->agregarEvento(ev);
}

void Nivel1::actualizar(float dt)
{
    if (completado) return;

    tiempoNivel += dt;

    // 1. Actualizar spawn
    gestorSpawn->actualizar(dt);

    // 2. Actualizar entidades
    for (EntidadJuego* entidad : entidades)
        entidad->actualizar(dt);

    // 3. Colisiones
    verificarColisiones();

    // 4. Entidades fuera de pantalla
    for (EntidadJuego* e : recogerEntidadesAEliminar())
        eliminarEntidad(e);

    // 5. Meta del nivel
    if (jugador &&
        jugador->obtenerDistanciaRecorrida() >= distanciaObjetivo)
    {
        completado = true;
        qDebug() << "Nivel 1 completado!";
    }

    // 6. Muerte del jugador
    if (jugador && jugador->obtenerVida() <= 0.0f)
    {
        completado = true;
        qDebug() << "Jugador eliminado. Nivel fallido.";
    }
}

bool Nivel1::nivelCompletado() const
{
    return completado;
}

// ---------------------------------------------------------
// SLOT: cuando ControlSpawn genera un enemigo
// ---------------------------------------------------------
void Nivel1::onEnemigoGenerado(Enemigo* nuevo)
{
    if (!nuevo) return;

    // Sin parent (manejo manual de memoria)
    nuevo->setParent(nullptr);

    entidades.append(nuevo);

    qDebug() << "Nivel1: enemigo spawn en"
             << nuevo->obtenerPosicion().x()
             << nuevo->obtenerPosicion().y();
}

// ---------------------------------------------------------
// Colisiones
// ---------------------------------------------------------
void Nivel1::verificarColisiones()
{
    if (!jugador) return;

    for (EntidadJuego* e : entidades)
    {
        if (e == jugador) continue;

        if (jugador->colisionaCon(e))
            procesarColisionJugador(e);
    }
}

void Nivel1::procesarColisionJugador(EntidadJuego* entidad)
{
    if (!entidad || !jugador) return;

    // 1. Enemigo
    if (auto* ene = dynamic_cast<Enemigo*>(entidad))
    {
        jugador->recibirDano(ene->obtenerDano());
        eliminarEntidad(ene);
        return;
    }

    // 2. Obstáculo fijo
    if (auto* fija = dynamic_cast<EntidadFija*>(entidad))
    {
        jugador->recibirDano(fija->obtenerDano());

        if (fija->obtenerEsDestructible())
            eliminarEntidad(fija);

        return;
    }
}

// ---------------------------------------------------------
// Eliminación de entidades
// ---------------------------------------------------------
QVector<EntidadJuego*> Nivel1::recogerEntidadesAEliminar()
{
    QVector<EntidadJuego*> lista;
    const float limiteAbajo = 850.0f;

    for (EntidadJuego* e : entidades)
    {
        if (e == jugador) continue;

        if (auto* ene = dynamic_cast<Enemigo*>(e))
        {
            if (ene->obtenerPosicion().y() > limiteAbajo)
                lista.append(e);
        }
    }
    return lista;
}

void Nivel1::eliminarEntidad(EntidadJuego* entidad)
{
    if (!entidad) return;

    int idx = entidades.indexOf(entidad);
    if (idx != -1)
        entidades.removeAt(idx);

    delete entidad;
}

