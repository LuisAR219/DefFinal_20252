#include "NivelLondres.h"
#include "Proyectil.h"
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
}

Soldado* NivelLondres::getJugador() const {
    return soldadoJugador;
}

int NivelLondres::getExplosionesRecibidas() const {
    return explosionesRecibidas;
}

void NivelLondres::setSpriteCache(const QMap<QString, QPixmap>& cache) {
    spriteCache = cache;
}

void NivelLondres::resetearExplosiones()
{
    explosionesRecibidas = 0;
    qDebug() << "Contador de explosiones reseteado a 0";
}

void NivelLondres::actualizar(float dt) {
    tiempoDisparo += dt;

    for (EntidadJuego* e : entidades) {
        e->actualizar(dt);
    }

    if (tiempoDisparo >= 1.0f) {
        tiempoDisparo = 0.0f;

        Proyectil* bomba = new Proyectil(nullptr);
        bomba->setPosicion(bombarderoEnemigo->getPosicion());
        bomba->setVelocidad(QVector2D(0, 250));
        bomba->setTipoEntidad(ENEMIGO);
        entidades.append(bomba);
    }

    QList<EntidadJuego*> aEliminar;

    for (EntidadJuego* e : entidades) {
        if (e == soldadoJugador) continue;

        QSize tamañoSoldado(109, 100);
        QSize tamañoEntidad = dynamic_cast<Proyectil*>(e)
                                  ? QSize(30, 30)
                                  : QSize(109, 100);

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

            qDebug() << "Explosión" << explosionesRecibidas << "/" << maxExplosionesPermitidas;

            if (dynamic_cast<Proyectil*>(e)) {
                aEliminar.append(e);
            }

            if (explosionesRecibidas >= maxExplosionesPermitidas) {
                qDebug() << "Límite alcanzado: emitiendo nivelFallido";
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
