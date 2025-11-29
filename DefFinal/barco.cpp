#include "barco.h"
#include <QPainter>

Barco::Barco(QObject* parent, int tipo_, const QVector2D& posInicial)
    : EntidadJuego(parent, posInicial, 1.0f, 30.0f, ENEMIGO),
    tipo(tipo_)
{
    destruido = false;
    // Configuración según tipo
    switch (tipo) {
    case 0: // pequeño
        vida = 30.0f;
        velocidadX = -70.0f;
        radioColision = 22.0f;
        anchoSprite = 80.0f;
        altoSprite = 30.0f;
        break;
    case 1: // mediano
        vida = 60.0f;
        velocidadX = -50.0f;
        radioColision = 34.0f;
        anchoSprite = 120.0f;
        altoSprite = 40.0f;
        break;
    case 2: // grande
    default:
        vida = 120.0f;
        velocidadX = -30.0f;
        radioColision = 48.0f;
        anchoSprite = 160.0f;
        altoSprite = 48.0f;
        break;
    }
}

void Barco::actualizar(float dt)
{
    // Movimiento horizontal simple hacia la izquierda
    posicion.setX(posicion.x() + velocidadX * dt);

    if (posicion.x() + anchoSprite < -100.0f)
        emit solicitudEliminar(this);
}

void Barco::aplicarFuerza(const QVector2D& fuerza)
{
    Q_UNUSED(fuerza);
}

bool Barco::colisionaCon(const EntidadJuego* otra) const
{
    float dist = (otra->obtenerPosicion() - posicion).length();
    return dist < (radioColision + otra->obtenerRadioColision());
}

void Barco::pintar(QPainter* pintor)
{
    float x = posicion.x() - anchoSprite/2.0f;
    float y = posicion.y() - altoSprite/2.0f;

    QColor color;
    if (tipo == 0) color = QColor(200, 100, 50); // pequeño - naranja
    else if (tipo == 1) color = QColor(120, 180, 200); // mediano - celeste
    else color = QColor(100, 100, 160); // grande - azul oscuro

    pintor->setBrush(color);
    pintor->setPen(Qt::black);
    pintor->drawRect(QRectF(x, y, anchoSprite, altoSprite));

    // Barra de vida encima del barco
    float barraW = anchoSprite * 0.8f;
    float barraH = 6.0f;
    float barraX = posicion.x() - barraW/2.0f;
    float barraY = y - 12.0f;

    // Fondo de la barra
    pintor->setBrush(Qt::darkGray);
    pintor->drawRect(barraX, barraY, barraW, barraH);

    // Porción según vida
    float fraccion = qMax(0.0f, vida / (tipo==0 ? 30.0f : (tipo==1 ? 60.0f : 120.0f)));
    pintor->setBrush(Qt::green);
    pintor->drawRect(barraX, barraY, barraW * fraccion, barraH);
}

void Barco::recibirDanio(float cantidad)
{
    vida -= cantidad;

    if (vida <= 0)
    {
        vida = 0;
        destruido = true;
    }
}
