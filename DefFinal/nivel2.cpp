#include "nivel2.h"
#include <QDebug>
#include <QtMath>

Nivel2::Nivel2(QObject* parent)
    : Nivel(parent),
    totalBarcosPorDerrotar(0),
    barcosDerrotados(0),
    barcosGenerados(0),
    maxSimultaneos(2),
    tiempoDesdeUltimoSpawn(0.0f),
    intervaloSpawnMin(1.0f),
    intervaloSpawnMax(3.0f),
    tiempoProximoSpawn(0.0f)
{
}

Nivel2::~Nivel2()
{
    for (EntidadJuego* e : entidadesNivel) delete e;
    entidadesNivel.clear();
}

void Nivel2::inicializar()
{
    qDebug() << "Inicializando Nivel 2 – Batalla de Pearl Harbor";

    // Reiniciamos variables
    tiempoDesdeUltimoSpawn = 0.0f;
    barcosDerrotados = 0;
    barcosGenerados = 0;
    entidades.clear();
    entidadesNivel.clear();
    completado = false;

    // 1) Determinar número total de barcos (3..6)
    totalBarcosPorDerrotar = 3 + QRandomGenerator::global()->bounded(4);
    qDebug() << "Total barcos a generar:" << totalBarcosPorDerrotar;

    // 2) Crear el cañón: posición base a la izquierda
    QVector2D posBase(120.0f, 180.0f);
    canon.inicializar(posBase, 80.0f, 420.0f);

    // 3) Generar hasta maxSimultaneos barcos iniciales
    while (barcosGenerados < qMin(maxSimultaneos, totalBarcosPorDerrotar)) {
        generarSiguienteBarco();
    }

    // planificar siguiente spawn
    tiempoProximoSpawn = intervaloSpawnMin + QRandomGenerator::global()->bounded(intervaloSpawnMax - intervaloSpawnMin);
}

void Nivel2::actualizar(float dt)
{
    if (completado) return;

    // 0) Actualizar temporizadores de spawn
    tiempoDesdeUltimoSpawn += dt;

    // 1) Actualizar cañón
    canon.actualizar(dt);

    // Si el cañón pidió un proyectil, generarlo y añadirlo
    ProyectilBalistico* p = canon.generarProyectilSiListo();
    if (p) {
        p->setParent(this);
        entidadesNivel.append(p);
        entidades.append(p);
    }

    // 2) Actualizar entidades (barcos y proyectiles)
    for (EntidadJuego* e : entidadesNivel) {
        e->actualizar(dt);
    }

    // 3) Manejar spawn
    int activos = contarBarcosActivos();
    if (barcosGenerados < totalBarcosPorDerrotar && activos < maxSimultaneos) {
        if (tiempoDesdeUltimoSpawn >= tiempoProximoSpawn) {
            generarSiguienteBarco();
            tiempoDesdeUltimoSpawn = 0.0f;
            tiempoProximoSpawn = intervaloSpawnMin + QRandomGenerator::global()->bounded(intervaloSpawnMax - intervaloSpawnMin);
        }
    }

    // 4) Colisiones proyectil-barco y daño por radio de explosión
    verificarColisiones();

    // 5) Recolectar y eliminar entidades que pidieron eliminación
    QVector<EntidadJuego*> aEliminar = recogerEntidadesAEliminar();
    for (EntidadJuego* en : aEliminar) eliminarEntidad(en);

    // 6) Condición de victoria
    if (barcosDerrotados >= totalBarcosPorDerrotar) {
        completado = true;
        qDebug() << "Nivel 2 completado!";
    }
}

bool Nivel2::nivelCompletado() const
{
    return completado;
}

void Nivel2::pintar(QPainter* painter)
{
    painter->setBrush(QColor(180,220,255)); // cielo
    painter->drawRect(0,0,800,600);

    // Línea de horizonte / agua
    painter->setBrush(QColor(50,120,200));
    painter->drawRect(0,420,800,180);

    // Pintar cañón
    canon.pintar(painter);

    // Pintar entidades del nivel (barcos y proyectiles)
    for (EntidadJuego* e : entidadesNivel) {
        e->pintar(painter);
    }

    // HUD: barcos restantes
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 14, QFont::Bold));
    painter->drawText(12, 20, QString("Barcos restantes: %1").arg(totalBarcosPorDerrotar - barcosDerrotados));
}

void Nivel2::procesarTeclaPress()
{
    canon.procesarPress();
}

void Nivel2::procesarTeclaRelease()
{
    canon.procesarRelease();
}

void Nivel2::generarSiguienteBarco()
{
    // Calcular tipo aleatorio 0..2
    int tipo = QRandomGenerator::global()->bounded(0, 3);

    // Posición inicial en la derecha, Y variable dentro del área del agua (p.ej. 440..520)
    float x = 900.0f + QRandomGenerator::global()->bounded(200);
    float y = 460.0f + QRandomGenerator::global()->bounded(60);

    Barco* b = new Barco(nullptr, tipo, QVector2D(x, y));
    b->setParent(this);
    entidadesNivel.append(b);
    entidades.append(b);

    ++barcosGenerados;
    qDebug() << "Spawn barco tipo" << tipo << "en X=" << x << "Y=" << y;
}

int Nivel2::contarBarcosActivos() const
{
    int c = 0;
    for (EntidadJuego* e : entidadesNivel) {
        if (dynamic_cast<Barco*>(e)) ++c;
    }
    return c;
}

void Nivel2::verificarColisiones()
{
    // Iterar sobre copias para evitar modificar vector en la iteración
    QVector<EntidadJuego*> lista = entidadesNivel;

    for (EntidadJuego* e : lista) {
        ProyectilBalistico* pb = dynamic_cast<ProyectilBalistico*>(e);
        if (pb && pb->estaActivo()) {
            for (EntidadJuego* e2 : lista) {
                Barco* b = dynamic_cast<Barco*>(e2);
                if (!b) continue;

                // Colisión directa entre proyectil y barco
                if (pb->colisionaCon(b)) {
                    // Impacto en el punto pb->posicion
                    QVector2D puntoImpacto = pb->obtenerPosicion();

                    // Calcular distancia al centro del barco
                    float dist = (b->obtenerPosicion() - puntoImpacto).length();
                    float R = pb->obtenerRadioExplosivo();
                    float D0 = pb->obtenerDanoBase();
                    float factor = qMax(0.0f, 1.0f - dist / R);
                    float danioAplicado = D0 * factor;

                    b->recibirDanio(danioAplicado);

                    // El proyectil hace explosion: eliminar proyectil
                    emit pb->solicitudEliminar(pb);

                    // Si barco destruido, incrementamos contador y planificamos generar otro posteriormente
                    if (b->estaDestruido()) {
                        ++barcosDerrotados;
                        // solicitar eliminación del barco
                        emit b->solicitudEliminar(b);
                    }

                    // Solo procesar primer barco alcanzado por este proyectil
                    break;
                }
            }
        }
    }
}

QVector<EntidadJuego*> Nivel2::recogerEntidadesAEliminar()
{
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

void Nivel2::eliminarEntidad(EntidadJuego* entidad)
{
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
