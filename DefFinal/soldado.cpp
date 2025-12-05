#include "Soldado.h"

Soldado::Soldado(QObject* parent)
    : EntidadJuego(parent, QVector2D(400,450), 1.0f, 30.0f, JUGADOR),
    factorInercia(0.85f),
    direccionActual(0, 0),
    velocidadMax(180.0f),
    invulnerableTimer(0.0f)
{
    setVida(100.0f);
}

void Soldado::aplicarFuerza(const QVector2D& fuerza) {
    setVelocidad(getVelocidad() + fuerza);
}

void Soldado::actualizar(float dt) {
    if (invulnerableTimer > 0.0f) {
        invulnerableTimer = qMax(0.0f, invulnerableTimer - dt);
    }

    QVector2D objetivo = direccionActual * velocidadMax;
    setVelocidad(getVelocidad() * factorInercia + objetivo * (1.0f - factorInercia));
    setPosicion(getPosicion() + getVelocidad() * dt);

    float x = qBound(50.0f, getPosicion().x(), 750.0f);
    float y = qBound(400.0f, getPosicion().y(), 550.0f);
    setPosicion(QVector2D(x, y));

    registrarPosicion();
}

bool Soldado::colisionaCon(const EntidadJuego* otra) const {
    float dist = (getPosicion() - otra->getPosicion()).length();
    return dist < (getRadioColision() + otra->getRadioColision());
}

void Soldado::pintar(QPainter* pintor) {
    QPixmap sprite(":/imagenes/Parado1.png");
    if (!sprite.isNull()) {
        QRectF rect(getPosicion().x() - 30, getPosicion().y() - 30, 60, 60);
        pintor->drawPixmap(rect.toRect(), sprite);
    } else {
        // Placeholder: círculo azul para el soldado
        pintor->setBrush(Qt::blue);
        pintor->setPen(Qt::NoPen);
        pintor->drawEllipse(QPointF(getPosicion().x(), getPosicion().y()),
                            getRadioColision(), getRadioColision());
    }
}

void Soldado::recibirInput(const QVector2D& direccion) {
    if (!qFuzzyIsNull(direccion.length())) {
        direccionActual = direccion.normalized();
    } else {
        direccionActual = QVector2D(0, 0);
    }
}

void Soldado::reaccionarAExplosión() {
    if (invulnerableTimer > 0.0f) return;

    invulnerableTimer = 0.5f;
    setVida(qMax(0.0f, getVida() - 25.0f));

    setVelocidad(getVelocidad() + QVector2D(-direccionActual.x(), -direccionActual.y()) * 80.0f);
}

const QQueue<QVector2D>& Soldado::getHistorialMovimiento() const {
    return historialMovimiento;
}


