#ifndef TANQUEJUGADOR_H
#define TANQUEJUGADOR_H

#include "entidadjuego.h"
#include <QSet>
#include <QVector2D>
#include <QPixmap>

class TanqueJugador : public EntidadJuego
{
    Q_OBJECT

public:
    explicit TanqueJugador(QObject* parent = nullptr,
                           const QVector2D& posicionInicial = QVector2D(0,0));

    void teclaPresionada(int key);
    void teclaLiberada(int key);

    void establecerLimites(float minX_, float maxX_, float minY_, float maxY_);

    float obtenerVida() const { return vida; }
    void recibirDano(float d) { vida -= d; if (vida < 0) vida = 0; }

    float obtenerDistanciaRecorrida() const { return distanciaRecorrida; }

    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

private:
    QSet<int> teclasActivas;

    float velocidadY;
    float velocidadX;
    float distanciaRecorrida;

    float minX, maxX, minY, maxY;
    float vida;

    QPixmap sprite;
    int anchoSprite;
    int altoSprite;
};

#endif // TANQUEJUGADOR_H
