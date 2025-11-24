#ifndef TANQUEJUGADOR_H
#define TANQUEJUGADOR_H

#include "entidadjuego.h"
#include <QSet>
#include <QVector2D>

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

    // Métodos obligatorios
    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;   // obligatorio
    bool colisionaCon(const EntidadJuego* otra) const override; // obligatorio
    void pintar(QPainter* pintor) override;  // obligatorio

private:
    QSet<int> teclasActivas;

    float velocidadY;
    float velocidadX;
    float distanciaRecorrida;

    float minX, maxX, minY, maxY;
    float vida;
};

#endif // TANQUEJUGADOR_H
