#include "barco.h"
#include <QPainter>

Barco::Barco(QObject* parent, int tipo_, const QVector2D& posInicial)
    : EntidadJuego(parent, posInicial, 1.0f, 30.0f, ENEMIGO),
    tipo(tipo_),
    destruido(false)
{
    switch (tipo) {
    case 0:
        vida = 30.0f;
        velocidadX = -70.0f;
        radioColision = 22.0f;
        anchoSprite = 80.0f;
        altoSprite = 30.0f;
        anchoHitbox = 80.0f;
        altoHitbox = 30.0f;
        break;
    case 1:
        vida = 60.0f;
        velocidadX = -50.0f;
        radioColision = 34.0f;
        anchoSprite = 120.0f;
        altoSprite = 40.0f;
        anchoHitbox = 120.0f;
        altoHitbox = 40.0f;
        break;
    case 2:
    default:
        vida = 120.0f;
        velocidadX = -30.0f;
        radioColision = 48.0f;
        anchoSprite = 160.0f;
        altoSprite = 48.0f;
        anchoHitbox = 160.0f;
        altoHitbox = 48.0f;
        break;
    }
}

void Barco::actualizar(float dt) {
    posicion.setX(posicion.x() + velocidadX * dt);

    if (posicion.x() + anchoSprite < -100.0f) {
        emit solicitudEliminar(this);
    }
}

bool Barco::colisionaCon(const EntidadJuego* otra) const {
    float anchoHit = anchoHitbox * 10.8f;
    float altoHit = altoHitbox * 10.2f;

    float dx = qAbs(otra->getPosicion().x() - posicion.x());
    float dy = qAbs(otra->getPosicion().y() - posicion.y());

    return dx <= (anchoHit / 2.0f) && dy <= (altoHit / 2.0f);
}

void Barco::pintar(QPainter* pintor) {
    float x = posicion.x() - anchoSprite/2.0f;
    float y = posicion.y() - altoSprite/2.0f;

    QColor color;
    if (tipo == 0) color = QColor(200, 100, 50);
    else if (tipo == 1) color = QColor(120, 180, 200);
    else color = QColor(100, 100, 160);

    pintor->setBrush(color);
    pintor->setPen(Qt::black);
    pintor->drawRect(QRectF(x, y, anchoSprite, altoSprite));

    float barraW = anchoSprite * 0.8f;
    float barraH = 6.0f;
    float barraX = posicion.x() - barraW/2.0f;
    float barraY = y - 12.0f;

    pintor->setBrush(Qt::darkGray);
    pintor->drawRect(barraX, barraY, barraW, barraH);

    float fraccion = qMax(0.0f, vida / (tipo==0 ? 30.0f : (tipo==1 ? 60.0f : 120.0f)));
    pintor->setBrush(Qt::green);
    pintor->drawRect(barraX, barraY, barraW * fraccion, barraH);
}

void Barco::recibirDanio(float cantidad) {
    vida -= cantidad;
    if (vida <= 0) {
        vida = 0;
        destruido = true;
    }
}
