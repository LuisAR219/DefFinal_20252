#include "nivel1.h"
#include <QDebug>
#include <QRandomGenerator>

Nivel1::Nivel1(QObject* parent)
    : Nivel(parent),
    jugador(nullptr),
    gestorSpawn(nullptr),
    tiempoNivel(0.0f),
    distanciaObjetivo(1800.0f),
    maxEnemigosActivos(3),
    tiempoDesdeUltimoSpawn(0.0f),
    intervaloSpawn(2.0f)
{
}

Nivel1::~Nivel1()
{
    for (EntidadJuego* e : entidades) delete e;
    entidades.clear();
}

void Nivel1::inicializar()
{
    qDebug() << "Inicializando Nivel 1 – Batalla de Kursk";

    tiempoNivel = 0.0f;
    completado = false;

    for (EntidadJuego* e : entidades) delete e;
    entidades.clear();

    // Jugador
    jugador = new TanqueJugador(nullptr, QVector2D(400.0f, 520.0f));
    jugador->establecerLimites(80.0f, 720.0f, 20.0f, 560.0f);
    entidades.append(jugador);

    // Spawn
    gestorSpawn = new ControlSpawn(this);
    connect(gestorSpawn, &ControlSpawn::enemigoGenerado,
            this, &Nivel1::onEnemigoGenerado);

    gestorSpawn->reiniciar();

    // Podemos dejar eventos iniciales (opcional)
    SpawnEvent ev;
    ev.tiempo = 1.0f; ev.posicion = QVector2D(100, 80); ev.velocidad = 40.0f; ev.dano = 15; ev.radio = 18.0f;
    gestorSpawn->agregarEvento(ev);

    // arranque: rellenar hasta maxEnemigosActivos con spawn aleatorio
    for (int i = 0; i < maxEnemigosActivos; ++i) {
        float x = 120.0f + QRandomGenerator::global()->bounded(560.0);
        QVector2D pos(x, 80.0f);
        gestorSpawn->generarEnemigoEn(pos);
    }
}

void Nivel1::actualizar(float dt)
{
    if (completado) return;

    tiempoNivel += dt;
    tiempoDesdeUltimoSpawn += dt;

    // 1. Actualizar spawn programado
    gestorSpawn->actualizar(dt);

    // 2. Actualizar entidades
    for (EntidadJuego* e : entidades) {
        e->actualizar(dt);
    }

    // 3. Colisiones
    verificarColisiones();

    // 4. Recolectar y eliminar entidades marcadas
    QVector<EntidadJuego*> aEliminar = recogerEntidadesAEliminar();
    for (EntidadJuego* en : aEliminar) eliminarEntidad(en);

    // 5. Mantener hasta maxEnemigosActivos (si hay menos y pasó intervalo)
    int activos = contarEnemigosActivos();
    if (activos < maxEnemigosActivos && tiempoDesdeUltimoSpawn >= intervaloSpawn) {
        float x = 120.0f + QRandomGenerator::global()->bounded(560.0);
        gestorSpawn->generarEnemigoEn(QVector2D(x, 80.0f));
        tiempoDesdeUltimoSpawn = 0.0f;
    }

    // 6. Condición de victoria / derrota
    if (jugador && jugador->obtenerDistanciaRecorrida() >= distanciaObjetivo) {
        completado = true;
        qDebug() << "Nivel 1 completado!";
    }
    if (jugador && jugador->obtenerVida() <= 0.0f) {
        completado = true;
        qDebug() << "Jugador eliminado. Nivel fallido.";
    }
}

bool Nivel1::nivelCompletado() const { return completado; }

void Nivel1::onEnemigoGenerado(Enemigo* nuevo)
{
    if (!nuevo) return;

    // Nos conectamos a su señal de disparo
    connect(nuevo, &Enemigo::disparoGenerado, this, &Nivel1::onDisparoGenerado);

    // Asegurarse que nivel sea dueño de la memoria
    nuevo->setParent(nullptr);
    entidades.append(nuevo);

    qDebug() << "Nivel1: enemigo spawn en"
             << nuevo->obtenerPosicion().x()
             << nuevo->obtenerPosicion().y();
}

void Nivel1::onDisparoGenerado(EntidadJuego* proyectil)
{
    if (!proyectil) return;

    // proyectil viene creado por Enemigo (Proyectil*)
    proyectil->setParent(nullptr);
    entidades.append(proyectil);
}

void Nivel1::verificarColisiones()
{
    if (!jugador) return;

    // Iterar sobre una copia porque podemos eliminar dentro del loop
    QVector<EntidadJuego*> lista = entidades;

    for (EntidadJuego* e : lista)
    {
        if (e == jugador) continue;

        // Si proyectil impacta jugador
        Proyectil* p = dynamic_cast<Proyectil*>(e);
        if (p) {
            if (p->colisionaCon(jugador)) {
                jugador->recibirDano(p->obtenerDano());
                eliminarEntidad(p);
                continue;
            }
        }

        // Enemigo colisiona con jugador (choque)
        Enemigo* en = dynamic_cast<Enemigo*>(e);
        if (en) {
            if (en->colisionaCon(jugador)) {
                jugador->recibirDano(en->obtenerDano());
                eliminarEntidad(en);
                continue;
            }
        }

        // Entidad fija colisiona con jugador
        EntidadFija* fija = dynamic_cast<EntidadFija*>(e);
        if (fija) {
            if (fija->colisionaCon(jugador)) {
                jugador->recibirDano(fija->obtenerDano());
                if (fija->obtenerEsDestructible()) eliminarEntidad(fija);
                continue;
            }
        }
    }
}

QVector<EntidadJuego*> Nivel1::recogerEntidadesAEliminar()
{
    QVector<EntidadJuego*> lista;

    const float limiteAbajo = 900.0f;

    for (EntidadJuego* e : entidades) {
        // No eliminar al jugador
        if (e == jugador) continue;

        // Eliminación por señal solicitudEliminar: revisamos el tipo (si fue marcado,
        // las entidades suelen emitir solicitudEliminar(this) y el nivel las borrará)
        // Como no tenemos un booleano de "marcado", checaremos salida por Y (proyectiles)
        if (auto* p = dynamic_cast<Proyectil*>(e)) {
            if (p->obtenerPosicion().y() > limiteAbajo) lista.append(e);
            continue;
        }

        // Enemigo: si fue eliminado por choque, no aparece aquí (se eliminó antes)
        // Entidades fijas no se eliminan por tiempo
    }

    return lista;
}

void Nivel1::eliminarEntidad(EntidadJuego* entidad)
{
    if (!entidad) return;

    int idx = entidades.indexOf(entidad);
    if (idx != -1) entidades.removeAt(idx);

    delete entidad;
}

int Nivel1::contarEnemigosActivos() const
{
    int c = 0;
    for (EntidadJuego* e : entidades)
        if (dynamic_cast<Enemigo*>(e)) ++c;
    return c;
}
