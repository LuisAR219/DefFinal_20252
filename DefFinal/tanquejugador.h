#ifndef TANQUEJUGADOR_H
#define TANQUEJUGADOR_H

#include "entidadjuego.h"
#include "controltanque.h"
#include <QPainter>

class TanqueJugador : public EntidadJuego
{
    Q_OBJECT

public:
    explicit TanqueJugador(QObject* parent = nullptr,
                           const QVector2D& posicionInicial = QVector2D(0.0f, 0.0f),
                           float masaInicial = 1.0f,
                           float radioColisionInicial = 16.0f);

    virtual ~TanqueJugador();

    // Interfaz EntidadJuego
    void actualizar(float dt) override;
    void aplicarFuerza(const QVector2D& fuerza) override;
    bool colisionaCon(const EntidadJuego* otra) const override;
    void pintar(QPainter* pintor) override;

    // Control del jugador (se usan dos controles: vertical y lateral)
    ControlTanque* obtenerControlVertical() const;
    ControlTanque* obtenerControlLateral() const;

    // Vida / daño
    void recibirDano(int dano);
    float obtenerVida() const;
    void establecerVida(float valor);

    // Distancia recorrida
    float obtenerDistanciaRecorrida() const;

    // Límites de movimiento (carretera)
    void establecerLimites(float izquierdo, float derecho, float superior, float inferior);

    // Configuración de máximos
    void establecerVelocidadMaxima(float maxX, float maxY);

private:
    // Componentes de control (propiedad del tanque)
    ControlTanque* controlVertical;
    ControlTanque* controlLateral;

    // Estadísticas del tanque
    float vida;
    float distanciaRecorrida;

    // Límites de la carretera / pantalla
    float limiteIzquierdo;
    float limiteDerecho;
    float limiteSuperior;
    float limiteInferior;

    // Velocidades máximas por eje
    float velocidadMaxX;
    float velocidadMaxY;

    // Método interno para aplicar límites y correcciones
    void aplicarLimites();
};

#endif // TANQUEJUGADOR_H

