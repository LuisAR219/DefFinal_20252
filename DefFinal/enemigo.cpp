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
    // Patrulla horizontal simple
    float nuevaX = posicion.x() + direccion * velocidadPatrulla * dt;

    // Si excede el rango, invertir dirección
    if (nuevaX > origenX + rangoPatrulla) {
        nuevaX = origenX + rangoPatrulla;
        direccion = -1;
    }
    else if (nuevaX < origenX - rangoPatrulla) {
        nuevaX = origenX - rangoPatrulla;
        direccion = 1;
    }

    posicion.setX(nuevaX);

    // Mantiene su altura (no baja) — si quieres que baje, suma aquí a Y

    // Manejo de disparo
    tiempoDesdeUltimoDisparo += dt;
    if (tiempoDesdeUltimoDisparo >= tiempoProximoDisparo) {
        // Crear proyectil que dispare hacia abajo desde la posición del enemigo
        QVector2D posBala(posicion.x(), posicion.y() + radioColision + 6.0f);
        Proyectil* bala = new Proyectil(nullptr, posBala, QVector2D(0.0f, 1.0f),
                                        260.0f, // velocidad fija (puedes usar constante)
                                        10,     // daño por tu elección
                                        6.0f);  // radio de la bala

        emit disparoGenerado(bala);

        tiempoDesdeUltimoDisparo = 0.0f;
        tiempoProximoDisparo = generarProximoDisparo();
    }

    // Si enemigo sale del área por X (no esperable) o por Y, se puede eliminar.
    // Aquí no eliminamos por Y porque son fijos en altura.
}

void Enemigo::aplicarFuerza(const QVector2D&)
{
    // No usado
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

void Enemigo::pintar(QPainter* pintor)
{
    pintor->save();

    // Cuerpo
    pintor->setPen(Qt::black);
    pintor->setBrush(Qt::red);
    float d = radioColision * 2.0f;
painter:; // <- placeholder removed
    pintor->drawEllipse(QRectF(posicion.x() - radioColision,
                               posicion.y() - radioColision,
                               d, d));

    // Ojo/cañón
    pintor->setBrush(Qt::darkRed);
    pintor->drawRect(QRectF(posicion.x() - 4.0f, posicion.y() - 2.0f, 8.0f, 6.0f));

    pintor->restore();
}


