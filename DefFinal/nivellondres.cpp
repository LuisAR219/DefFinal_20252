#include "NivelLondres.h"
#include "Proyectil.h"
#include "MotorAprendizaje.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QRectF>

NivelLondres::NivelLondres(QObject* parent)
    : Nivel(parent), soldadoJugador(nullptr), bombarderoEnemigo(nullptr), explosionesRecibidas(0)
{}

void NivelLondres::inicializar() {
    soldadoJugador = new Soldado(this);
    bombarderoEnemigo = new AvionEnemigo(this);

    soldadoJugador->setPosicion(QVector2D(400, 450));
    bombarderoEnemigo->setPosicion(QVector2D(400, 0));

    entidades.append(soldadoJugador);
    entidades.append(bombarderoEnemigo);

    tiempoDisparo = 0.0f;
    explosionesRecibidas = 0;
    tiempoIA = 0.0f;
    historialJugador.clear();

    connect(soldadoJugador, &Soldado::posicionRegistrada, this, &NivelLondres::registrarPosicionJugador);
}

void NivelLondres::actualizar(float dt) {
    tiempoDisparo += dt;
    tiempoIA += dt;

    // 1) Registrar movimiento cada 0.5s
    if (tiempoIA >= intervaloIA) {
        tiempoIA = 0.0f;
        motorIA.analizarMovimiento(historialJugador);
    }

    // 2) Actualizar entidades
    for (EntidadJuego* e : entidades) e->actualizar(dt);

    // 3) Bombas con predicción
    if (tiempoDisparo >= 1.0f) {
        tiempoDisparo = 0.0f;

        QVector2D posJugador = soldadoJugador->getPosicion();
        QVector2D prediccion = motorIA.getDireccionPredicha() * 80.0f; // 80 px de anticipación
        QVector2D bombaDestino = posJugador + prediccion;

        Proyectil* bomba = new Proyectil(
            nullptr,
            bombarderoEnemigo->getPosicion(),
            (bombaDestino - bombarderoEnemigo->getPosicion()).normalized() * 250.0f,
            250.0f,
            25.0f,
            30.0f
            );
        bomba->setTipoEntidad(ENEMIGO);
        entidades.append(bomba);
    }

    // 4) Colisiones
    QList<EntidadJuego*> aEliminar;
    for (EntidadJuego* e : entidades) {
        if (e == soldadoJugador) continue;

        QSize tamañoSoldado(60, 80);
        QSize tamañoEntidad = dynamic_cast<Proyectil*>(e) ? QSize(30, 30) : QSize(60, 80);

        QRectF rectSoldado(
            soldadoJugador->getPosicion().x() - tamañoSoldado.width() / 2,
            soldadoJugador->getPosicion().y() - tamañoSoldado.height(),
            tamañoSoldado.width(),
            tamañoSoldado.height()
            );

        QRectF rectEntidad(
            e->getPosicion().x() - tamañoEntidad.width() / 2,
            e->getPosicion().y() - tamañoEntidad.height(),
            tamañoEntidad.width(),
            tamañoEntidad.height()
            );

        if (rectSoldado.intersects(rectEntidad)) {
            soldadoJugador->reaccionarAExplosión();
            explosionesRecibidas++;

            if (dynamic_cast<Proyectil*>(e)) aEliminar.append(e);

            if (explosionesRecibidas >= maxExplosionesPermitidas) {
                temporizadorNivel.stop();
                emit nivelFallido();
                return;
            }
        }
    }

    for (EntidadJuego* e : aEliminar) {
        entidades.removeOne(e);
        delete e;
    }
}

bool NivelLondres::nivelCompletado() const {
    return completado;
}

int NivelLondres::getExplosionesRecibidas() const {
    return explosionesRecibidas;
}

void NivelLondres::resetearExplosiones() {
    explosionesRecibidas = 0;
}

void NivelLondres::registrarPosicionJugador(const QVector2D& pos)
{
    historialJugador.enqueue(pos);
    const int maxMuestras = 60;
    while (historialJugador.size() > maxMuestras)
        historialJugador.dequeue();
}

void Soldado::registrarPosicion() {
    historialMovimiento.enqueue(getPosicion());
    const int maxMuestras = 60;
    while (historialMovimiento.size() > maxMuestras) {
        historialMovimiento.dequeue();
    }
    emit posicionRegistrada(getPosicion());
}
