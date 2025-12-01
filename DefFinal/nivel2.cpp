#include "nivel2.h"
#include <QDebug>
#include <QtMath>

Nivel2::Nivel2(QObject* parent)
    : Nivel(parent)
{
    barcosDerrotados = 0;
    barcosGenerados  = 0;
    totalBarcosPorDerrotar = 8 + QRandomGenerator::global()->bounded(5);
    maxSimultaneos           = 4;
    tiempoDesdeUltimoSpawn   = 0.0f;
    intervaloSpawnMin        = 1.0f;
    intervaloSpawnMax        = 1.0f;
    tiempoProximoSpawn       = 1.0f;
}

Nivel2::~Nivel2() {
    for (EntidadJuego* e : entidadesNivel) delete e;
    entidadesNivel.clear();
}

void Nivel2::inicializar() {
    qDebug() << "Inicializando Nivel 2 – Batalla de Pearl Harbor";

    tiempoDesdeUltimoSpawn = 0.0f;
    barcosDerrotados = 0;
    barcosGenerados = 0;
    entidades.clear();
    entidadesNivel.clear();
    completado = false;

    totalBarcosPorDerrotar = 3 + QRandomGenerator::global()->bounded(4);
    qDebug() << "Total barcos a generar:" << totalBarcosPorDerrotar;

    QVector2D posBase(120.0f, 180.0f);
    canon.inicializar(posBase, 80.0f, 420.0f);

    while (barcosGenerados < qMin(maxSimultaneos, totalBarcosPorDerrotar)) {
        generarSiguienteBarco();
    }

    tiempoProximoSpawn = intervaloSpawnMin + QRandomGenerator::global()->bounded(intervaloSpawnMax - intervaloSpawnMin);
}

void Nivel2::actualizar(float dt) {
    if (completado) return;

    tiempoDesdeUltimoSpawn += dt;
    canon.actualizar(dt);

    ProyectilBalistico* p = canon.generarProyectilSiListo();
    if (p) {
        p->setParent(this);
        entidadesNivel.append(p);
        entidades.append(p);
    }

    for (EntidadJuego* e : entidadesNivel) {
        e->actualizar(dt);
    }

    int activos = contarBarcosActivos();
    if (barcosGenerados < totalBarcosPorDerrotar && activos < maxSimultaneos) {
        if (tiempoDesdeUltimoSpawn >= tiempoProximoSpawn) {
            generarSiguienteBarco();
            tiempoDesdeUltimoSpawn = 0.0f;
            tiempoProximoSpawn = intervaloSpawnMin + QRandomGenerator::global()->bounded(intervaloSpawnMax - intervaloSpawnMin);
        }
    }

    verificarColisiones();

    QVector<EntidadJuego*> aEliminar = recogerEntidadesAEliminar();
    for (EntidadJuego* en : aEliminar) eliminarEntidad(en);

    if (barcosDerrotados >= totalBarcosPorDerrotar) {
        completado = true;
        qDebug() << "Nivel 2 completado!";
        emit nivelCompletado();
    }
}

bool Nivel2::nivelCompletado() const {
    return completado;
}

void Nivel2::pintar(QPainter* painter) {
    painter->setBrush(QColor(180,220,255));
    painter->drawRect(0,0,800,600);
    painter->setBrush(QColor(50,120,200));
    painter->drawRect(0,420,800,180);
    canon.pintar(painter);

    for (EntidadJuego* e : entidadesNivel) {
        e->pintar(painter);
    }

    painter->setPen(QPen(Qt::white)); // <─ corregido
    painter->setFont(QFont("Arial", 14, QFont::Bold));
    painter->drawText(12, 20, QString("Barcos restantes: %1").arg(totalBarcosPorDerrotar - barcosDerrotados));
}

void Nivel2::procesarTeclaPress() {
    canon.procesarPress();
}

void Nivel2::procesarTeclaRelease() {
    canon.procesarRelease();
}

void Nivel2::generarSiguienteBarco() {
    int tipo = QRandomGenerator::global()->bounded(0, 3);
    float x = 900.0f + QRandomGenerator::global()->bounded(200);
    float y = 460.0f + QRandomGenerator::global()->bounded(60);

    Barco* b = new Barco(nullptr, tipo, QVector2D(x, y));
    b->setParent(this);
    entidadesNivel.append(b);
    entidades.append(b);

    ++barcosGenerados;
    qDebug() << "Spawn barco tipo" << tipo << "en X=" << x << "Y=" << y;
}

int Nivel2::contarBarcosActivos() const {
    int c = 0;
    for (EntidadJuego* e : entidadesNivel) {
        if (dynamic_cast<Barco*>(e)) ++c;
    }
    return c;
}

void Nivel2::verificarColisiones() {
    QVector<EntidadJuego*> lista = entidadesNivel;

    for (EntidadJuego* e : lista) {
        ProyectilBalistico* pb = dynamic_cast<ProyectilBalistico*>(e);
        if (pb && pb->estaActivo()) {
            for (EntidadJuego* e2 : lista) {
                Barco* b = dynamic_cast<Barco*>(e2);
                if (!b) continue;

                if (pb->colisionaCon(b)) {
                    QVector2D puntoImpacto = pb->getPosicion();
                    float dist = (b->getPosicion() - puntoImpacto).length();
                    float R = pb->obtenerRadioExplosivo();
                    float D0 = pb->obtenerDanoBase();
                    float factor = qMax(0.0f, 1.0f - dist / R);
                    float danioAplicado = D0 * factor;

                    b->recibirDanio(danioAplicado);
                    emit pb->solicitudEliminar(pb);

                    if (b->estaDestruido()) {
                        ++barcosDerrotados;
                        emit b->solicitudEliminar(b);
                    }
                    break;
                }
            }
        }
    }
}

QVector<EntidadJuego*> Nivel2::recogerEntidadesAEliminar() {
    QVector<EntidadJuego*> lista;
    for (EntidadJuego* e : entidadesNivel) {
        if (auto* pb = dynamic_cast<ProyectilBalistico*>(e)) {
            if (!pb->estaActivo()) lista.append(e);
        }
        else if (auto* b = dynamic_cast<Barco*>(e)) {
            if (b->estaDestruido()) lista.append(e);
        }
    }
    return lista;
}

void Nivel2::eliminarEntidad(EntidadJuego* entidad) {
    if (!entidad) return;

    int idx = entidadesNivel.indexOf(entidad);
    if (idx != -1) entidadesNivel.removeAt(idx);

    int idx2 = entidades.indexOf(entidad);
    if (idx2 != -1) entidades.removeAt(idx2);

    delete entidad;
}

Canon* Nivel2::obtenerCanon() {
    return &canon;
}
