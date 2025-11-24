#include "nivel1.h"
#include <QDebug>
#include <QRandomGenerator>

Nivel1::Nivel1(QObject* parent)
    : Nivel(parent),
    jugador(nullptr),
    gestorSpawn(nullptr),
    tiempoNivel(0.0f),
    distanciaObjetivo(1800.0f),
    maxEnemigosActivos(10),
    tiempoDesdeUltimoSpawn(0.0f),
    intervaloSpawn(2.0f),
    tiempoDesdeUltimoObstaculo(0.0f),
    intervaloObstaculos(1.0f)
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
    tiempoDesdeUltimoSpawn = 0.0f;
    tiempoDesdeUltimoObstaculo = 0.0f;
    completado = false;

    for (EntidadJuego* e : entidades) delete e;
    entidades.clear();

    // Jugador
    jugador = new TanqueJugador(nullptr, QVector2D(400.0f, 520.0f));
    jugador->establecerLimites(80.0f, 720.0f, 20.0f, 560.0f);
    entidades.append(jugador);

    // Spawn de enemigos (gestionado por ControlSpawn)
    gestorSpawn = new ControlSpawn(this);
    connect(gestorSpawn, &ControlSpawn::enemigoGenerado,
            this, &Nivel1::onEnemigoGenerado);

    gestorSpawn->reiniciar();

    // Eventos iniciales
    SpawnEvent ev;
    ev.tiempo = 1.0f; ev.posicion = QVector2D(100, 80); ev.velocidad = 40.0f; ev.dano = 15; ev.radio = 18.0f;
    gestorSpawn->agregarEvento(ev);

    // Rellenar hasta maxEnemigosActivos
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
    tiempoDesdeUltimoObstaculo += dt;

    // 1) Actualizar spawn programado de enemigos
    if (gestorSpawn) gestorSpawn->actualizar(dt);

    // 2) Spawn periódico de obstaculos cada intervaloObstaculos
    if (tiempoDesdeUltimoObstaculo >= intervaloObstaculos) {
        tiempoDesdeUltimoObstaculo = 0.0f;

        // Generar tipo aleatorio 0..2
        int t = QRandomGenerator::global()->bounded(0, 3);
        Obstaculo::Tipo tipo = static_cast<Obstaculo::Tipo>(t);

        // Posición X aleatoria en rango 80..720
        float x = 80.0f + QRandomGenerator::global()->bounded(640);

        Obstaculo* obs = new Obstaculo(tipo, nullptr, QVector2D(x, 0.0f));
        obs->setParent(nullptr);
        entidades.append(obs);
        qDebug() << "Nivel1: obstaculo tipo" << t << "spawn en X=" << x;
    }

    // 3) Mantener número mínimo de enemigos
    int activos = contarEnemigosActivos();
    if (activos < maxEnemigosActivos && tiempoDesdeUltimoSpawn >= intervaloSpawn) {
        float x = 120.0f + QRandomGenerator::global()->bounded(560.0);
        gestorSpawn->generarEnemigoEn(QVector2D(x, 80.0f));
        tiempoDesdeUltimoSpawn = 0.0f;
    }

    // 4) Actualizar entidades
    for (EntidadJuego* e : entidades) {
        e->actualizar(dt);
    }

    // 5) Colisiones
    verificarColisiones();

    // 6) Recolectar y eliminar entidades fuera de pantalla
    QVector<EntidadJuego*> aEliminar = recogerEntidadesAEliminar();
    for (EntidadJuego* en : aEliminar) eliminarEntidad(en);

    // 7) Victoria / derrota
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

    connect(nuevo, &Enemigo::disparoGenerado, this, &Nivel1::onDisparoGenerado);
    nuevo->setParent(nullptr);
    entidades.append(nuevo);

    qDebug() << "Nivel1: enemigo spawn en"
             << nuevo->obtenerPosicion().x()
             << nuevo->obtenerPosicion().y();
}

void Nivel1::onDisparoGenerado(EntidadJuego* proyectil)
{
    if (!proyectil) return;
    proyectil->setParent(nullptr);
    entidades.append(proyectil);
}

// Colisiones: añade manejo de Obstaculo además de los demás tipos
void Nivel1::verificarColisiones()
{
    if (!jugador) return;

    // Hacemos copia porque podemos eliminar durante la iteración
    QVector<EntidadJuego*> lista = entidades;

    for (EntidadJuego* e : lista)
    {
        if (e == jugador) continue;

        // Proyectil que pega al jugador
        if (auto* p = dynamic_cast<Proyectil*>(e)) {
            if (p->colisionaCon(jugador)) {
                jugador->recibirDano(p->obtenerDano());
                eliminarEntidad(p);
                continue;
            }
        }

        // Obstáculo que choca con jugador
        if (auto* obs = dynamic_cast<Obstaculo*>(e)) {
            if (obs->colisionaCon(jugador)) {
                jugador->recibirDano(obs->obtenerDano());
                eliminarEntidad(obs);
                continue;
            }
        }

        // Enemigo que choca con jugador
        if (auto* en = dynamic_cast<Enemigo*>(e)) {
            if (en->colisionaCon(jugador)) {
                jugador->recibirDano(en->obtenerDano());
                eliminarEntidad(en);
                continue;
            }
        }

        // Entidad fija
        if (auto* fija = dynamic_cast<EntidadFija*>(e)) {
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
        if (e == jugador) continue;

        // Proyectiles fuera de pantalla
        if (auto* p = dynamic_cast<Proyectil*>(e)) {
            if (p->obtenerPosicion().y() > limiteAbajo) lista.append(e);
            continue;
        }

        // Obstaculos fuera de pantalla
        if (auto* obs = dynamic_cast<Obstaculo*>(e)) {
            if (obs->obtenerPosicion().y() > limiteAbajo) lista.append(e);
            continue;
        }
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

