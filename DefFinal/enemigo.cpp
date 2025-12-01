#include "enemigo.h"
#include "proyectil.h"
#include <QtMath>
#include <QRandomGenerator>

Enemigo::Enemigo(QObject* parent,
                 const QVector2D& posicionInicial,
                 float velocidadPatrulla_,
                 int danoChoque,
                 float radio)
    : EntidadJuego(parent, posicionInicial, 1.0f, radio, ENEMIGO),
    velocidadPatrulla(velocidadPatrulla_),
    dano(danoChoque),
    origenX(posicionInicial.x()),
    rangoPatrulla(140.0f),
    direccion(1),
    tiempoDesdeUltimoDisparo(0.0f),
    tiempoProximoDisparo(generarProximoDisparo())
{}

float Enemigo::generarProximoDisparo() {
    double r = QRandomGenerator::global()->generateDouble();
    return 1.0f + static_cast<float>(r) * 1.5f;
}

void Enemigo::actualizar(float dt) {
    float nuevaX = getPosicion().x() + direccion * velocidadPatrulla * dt;

    if (nuevaX > origenX + rangoPatrulla) {
        nuevaX = origenX + rangoPatrulla;
        direccion = -1;
    }
    else if (nuevaX < origenX - rangoPatrulla) {
        nuevaX = origenX - rangoPatrulla;
        direccion = 1;
    }

    setPosicion(QVector2D(nuevaX, getPosicion().y()));

    tiempoDesdeUltimoDisparo += dt;
    if (tiempoDesdeUltimoDisparo >= tiempoProximoDisparo) {
        QVector2D posBala(getPosicion().x(), getPosicion().y() + getRadioColision() + 6.0f);
        Proyectil* bala = new Proyectil(this, posBala, QVector2D(0.0f, 1.0f), 260.0f, 10, 6.0f);
        emit disparoGenerado(bala);
        tiempoDesdeUltimoDisparo = 0.0f;
        tiempoProximoDisparo = generarProximoDisparo();
    }
}

bool Enemigo::colisionaCon(const EntidadJuego* otra) const {
    float dist = (otra->getPosicion() - getPosicion()).length();
    return dist < (getRadioColision() + otra->getRadioColision());
}

void Enemigo::pintar(QPainter* p) {
    static QPixmap sprite(":/imagenes/tanqueEne.png");
    QRectF r(getPosicion().x() - 20, getPosicion().y() - 20, 40, 40);
    p->drawPixmap(r.toRect(), sprite);
}
