#include "AvionEnemigo.h"

AvionEnemigo::AvionEnemigo(QObject* parent)
    : EntidadMovil(parent, QVector2D(0,0), 0.1f, 40.0f, ENEMIGO),
    frecuenciaLanzamiento(1.5f),
    tiempoDesdeUltimoDisparo(0.0f),
    motorIA(nullptr),
    bombarderoDerecha(true)
{
}

void AvionEnemigo::actualizar(float dt) {
    float velocidadX = 100.0f;

    if (bombarderoDerecha) {
        setPosicion(QVector2D(getPosicion().x() + velocidadX * dt, getPosicion().y()));
        if (getPosicion().x() > 750) {
            bombarderoDerecha = false;
        }
    } else {
        setPosicion(QVector2D(getPosicion().x() - velocidadX * dt, getPosicion().y()));
        if (getPosicion().x() < 50) {
            bombarderoDerecha = true;
        }
    }

    tiempoDesdeUltimoDisparo += dt;
}

bool AvionEnemigo::verificaImpacto(const EntidadJuego* jugador) const {
    return (getPosicion() - jugador->getPosicion()).length() < 50.0f;
}

bool AvionEnemigo::colisionaCon(const EntidadJuego* otra) const {
    return (getPosicion() - otra->getPosicion()).length() < 40.0f;
}

void AvionEnemigo::inicializarMotorAprendizaje() {
    motorIA = new MotorAprendizaje();
}

void AvionEnemigo::adaptarEstrategia(const QQueue<QVector2D>& historialJugador) {
    if(motorIA) motorIA->analizarMovimiento(historialJugador);
}

Proyectil* AvionEnemigo::soltarBomba() {
    if(tiempoDesdeUltimoDisparo >= frecuenciaLanzamiento) {
        tiempoDesdeUltimoDisparo = 0.0f;
        Proyectil* bomba = new Proyectil(nullptr, getPosicion(), QVector2D(0,1), 150.0f, 25.0f, 30.0f);
        bomba->setTipoEntidad(ENEMIGO);
        return bomba;
    }
    return nullptr;
}

// IMPLEMENTACIÓN AÑADIDA
void AvionEnemigo::pintar(QPainter* pintor) {
    // Usar sprite del caché o placeholder
    QPixmap sprite(":/imagenes/Avion.png");
    if (!sprite.isNull()) {
        // Dibujar avión en la posición actual
        QRectF rect(getPosicion().x() - 40, getPosicion().y() - 20, 80, 40);
        pintor->drawPixmap(rect.toRect(), sprite);
    } else {
        // Placeholder: rectángulo rojo para el avión
        pintor->setBrush(Qt::red);
        pintor->setPen(Qt::NoPen);
        pintor->drawRect(QRectF(getPosicion().x() - 40, getPosicion().y() - 20, 80, 40));
    }
}
