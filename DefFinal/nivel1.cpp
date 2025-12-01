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

Nivel1::~Nivel1() {
    for (EntidadJuego* e : entidades) delete e;
    entidades.clear();
}

void Nivel1::inicializar() {
    qDebug() << "Inicializando Nivel 1 – Batalla de Kursk";

    tiempoNivel = 0.0f;
    tiempoDesdeUltimoSpawn = 0.0f;
    tiempoDesdeUltimoObstaculo = 0.0f;
    completado = false;

    // Limpiar entidades previas
    for (EntidadJuego* e : entidades) delete e;
    entidades.clear();

    // Crear jugador
    jugador = new TanqueJugador(nullptr, QVector2D(400.0f, 520.0f));
    jugador->establecerLimites(80.0f, 720.0f, 20.0f, 560.0f);
    entidades.append(jugador);

    // Configurar spawn de enemigos
    gestorSpawn = new ControlSpawn(this);
    connect(gestorSpawn, &ControlSpawn::enemigoGenerado, this, &Nivel1::onEnemigoGenerado);

    gestorSpawn->reiniciar();

    // Evento inicial de spawn
    SpawnEvent ev;
    ev.tiempo = 1.0f;
    ev.posicion = QVector2D(100, 80);
    ev.velocidad = 40.0f;
    ev.dano = 15;
    ev.radio = 18.0f;
    gestorSpawn->agregarEvento(ev);

    // Generar enemigos iniciales
    for (int i = 0; i < maxEnemigosActivos; ++i) {
        float x = 120.0f + QRandomGenerator::global()->bounded(560.0);
        QVector2D pos(x, 80.0f);
        gestorSpawn->generarEnemigoEn(pos);
    }
}

void Nivel1::actualizar(float dt) {
    if (completado) return;

    tiempoNivel += dt;
    tiempoDesdeUltimoSpawn += dt;
    tiempoDesdeUltimoObstaculo += dt;

    // 1) Spawn programado de enemigos
    if (gestorSpawn) gestorSpawn->actualizar(dt);

    // 2) Spawn periódico de obstáculos
    if (tiempoDesdeUltimoObstaculo >= intervaloObstaculos) {
        tiempoDesdeUltimoObstaculo = 0.0f;

        int t = QRandomGenerator::global()->bounded(0, 3);
        Obstaculo::Tipo tipo = static_cast<Obstaculo::Tipo>(t);
        float x = 80.0f + QRandomGenerator::global()->bounded(640);

        Obstaculo* obs = new Obstaculo(tipo, nullptr, QVector2D(x, 0.0f));
        obs->setParent(nullptr);

        // Conectar señal de eliminación
        connect(obs, &Obstaculo::solicitudEliminar, this, &Nivel1::eliminarEntidad);

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

    // 4) Actualizar todas las entidades
    for (EntidadJuego* e : entidades) {
        e->actualizar(dt);
    }

    // 5) Verificar colisiones
    verificarColisiones();

    // 6) Eliminar entidades fuera de pantalla
    QVector<EntidadJuego*> aEliminar = recogerEntidadesAEliminar();
    for (EntidadJuego* en : aEliminar) eliminarEntidad(en);

    // 7) Condiciones de victoria/derrota
    if (jugador && jugador->obtenerDistanciaRecorrida() >= distanciaObjetivo) {
        completado = true;
        qDebug() << "Nivel 1 completado!";
        emit nivelCompletado();
    }
    if (jugador && jugador->obtenerVida() <= 0.0f) {
        completado = true;
        qDebug() << "Jugador eliminado. Nivel fallido.";
        emit nivelFallido();
    }
}

void Nivel1::onEnemigoGenerado(Enemigo* nuevo) {
    if (!nuevo) return;

    connect(nuevo, &Enemigo::disparoGenerado, this, &Nivel1::onDisparoGenerado);
    nuevo->setParent(nullptr);
    entidades.append(nuevo);

    qDebug() << "Nivel1: enemigo spawn en"
             << nuevo->getPosicion().x()
             << nuevo->getPosicion().y();
}

void Nivel1::onDisparoGenerado(EntidadJuego* proyectil) {
    if (!proyectil) return;
    proyectil->setParent(nullptr);
    entidades.append(proyectil);
}

void Nivel1::verificarColisiones() {
    if (!jugador) return;

    QVector<EntidadJuego*> lista = entidades;

    for (EntidadJuego* e : lista) {
        if (e == jugador) continue;

        // Proyectil impacta jugador
        if (auto* p = dynamic_cast<Proyectil*>(e)) {
            if (p->colisionaCon(jugador)) {
                jugador->recibirDano(p->getDaño());
                eliminarEntidad(p);
                continue;
            }
        }

        // Obstáculo impacta jugador
        if (auto* obs = dynamic_cast<Obstaculo*>(e)) {
            if (obs->colisionaCon(jugador)) {
                jugador->recibirDano(obs->obtenerDano());
                eliminarEntidad(obs);
                continue;
            }
        }

        // Enemigo impacta jugador
        if (auto* en = dynamic_cast<Enemigo*>(e)) {
            if (en->colisionaCon(jugador)) {
                jugador->recibirDano(en->obtenerDano());
                eliminarEntidad(en);
                continue;
            }
        }

        // Entidad fija impacta jugador
        if (auto* fija = dynamic_cast<EntidadFija*>(e)) {
            if (fija->colisionaCon(jugador)) {
                jugador->recibirDano(fija->obtenerDano());
                if (fija->obtenerEsDestructible()) eliminarEntidad(fija);
                continue;
            }
        }
    }
}

QVector<EntidadJuego*> Nivel1::recogerEntidadesAEliminar() {
    QVector<EntidadJuego*> lista;
    const float limiteAbajo = 900.0f;

    for (EntidadJuego* e : entidades) {
        if (e == jugador) continue;

        if (auto* p = dynamic_cast<Proyectil*>(e)) {
            if (p->getPosicion().y() > limiteAbajo) lista.append(e);
            continue;
        }

        if (auto* obs = dynamic_cast<Obstaculo*>(e)) {
            if (obs->getPosicion().y() > limiteAbajo) lista.append(e);
            continue;
        }
    }

    return lista;
}

void Nivel1::eliminarEntidad(EntidadJuego* entidad) {
    if (!entidad) return;

    int idx = entidades.indexOf(entidad);
    if (idx != -1) entidades.removeAt(idx);

    delete entidad;
}

int Nivel1::contarEnemigosActivos() const {
    int c = 0;
    for (EntidadJuego* e : entidades)
        if (dynamic_cast<Enemigo*>(e)) ++c;
    return c;
}
