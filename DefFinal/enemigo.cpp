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
    tiempoDesdeUltimoDisparo(0.0f)
{
    tiempoProximoDisparo = generarProximoDisparo();
}

float Enemigo::generarProximoDisparo()
{
    // Random entre 1.0 y 2.5
    double r = QRandomGenerator::global()->generateDouble();
    return 1.0f + static_cast<float>(r) * 1.5f;
}

void Enemigo::actualizar(float dt)
{
    float nuevaX = posicion.x() + direccion * velocidadPatrulla * dt;

    if (nuevaX > origenX + rangoPatrulla) {
        nuevaX = origenX + rangoPatrulla;
        direccion = -1;
    }
    else if (nuevaX < origenX - rangoPatrulla) {
        nuevaX = origenX - rangoPatrulla;
        direccion = 1;
    }

    posicion.setX(nuevaX);

    tiempoDesdeUltimoDisparo += dt;
    if (tiempoDesdeUltimoDisparo >= tiempoProximoDisparo) {
        QVector2D posBala(posicion.x(), posicion.y() + radioColision + 6.0f);
        Proyectil* bala = new Proyectil(nullptr, posBala, QVector2D(0.0f, 1.0f),
                                        260.0f, // velocidad fija
                                        10,     // daño
                                        6.0f);  // radio de la bala

        emit disparoGenerado(bala);

        tiempoDesdeUltimoDisparo = 0.0f;
        tiempoProximoDisparo = generarProximoDisparo();
    }
}

void Enemigo::aplicarFuerza(const QVector2D&)
{
}

bool Enemigo::colisionaCon(const EntidadJuego* otra) const
{
    float dist = (otra->obtenerPosicion() - posicion).length();
    return dist < (radioColision + otra->obtenerRadioColision());
}

int Enemigo::obtenerDano() const
{
    return dano;
}

void Enemigo::pintar(QPainter* p)
{
    static QPixmap sprite(":/imagenes/tanqueEne.png");

    QRectF r(posicion.x() - 40/2,
             posicion.y() - 40/2,
             60,
             60);

    p->drawPixmap(r.toRect(), sprite);

}
