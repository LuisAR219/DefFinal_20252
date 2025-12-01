#ifndef CANON_H
#define CANON_H

#include <QPainter>
#include <QVector2D>
#include "proyectilbalistico.h"

class Canon
{
public:
    Canon();

    void inicializar(const QVector2D& posBase, float alturaMin, float alturaMax);

    void actualizar(float dt);
    void pintar(QPainter* painter);

    void procesarPress();
    void procesarRelease();

    ProyectilBalistico* generarProyectilSiListo();

    // Getters
    QVector2D obtenerPosicionBoca() const;
    float obtenerAnguloRad() const;
    float obtenerPotenciaNormalizada() const;
    void procesarTecla(int tecla, bool presionada);

private:
    // Parámetros físicos
    QVector2D posicionBase; // punto fijo donde se apoya el cañón (x,y)
    float minAltura;
    float maxAltura;

    // Movimiento vertical (oscilación)
    float tiempoAcumulado;
    float amplitud;
    float periodo; // periodo en segundos

    // Altura actual (Y absoluta en píxeles)
    float alturaActual;

    // Ángulo (radianes) = 0..pi/2
    float anguloActual;
    float anguloMin;
    float anguloMax;
    float anguloPeriodo; // tiempo para un ciclo de ángulo

    // Potencia (0..1)
    float potenciaNorm;
    float potenciaDireccional; // 1 = subiendo, -1 = bajando
    float potenciaPeriodo;

    // Estado
    int estado;
    bool teclaPresionada;
    bool listoParaDisparo;

    // Parámetros de proyectil por defecto
    float velocidadMin;
    float velocidadMax;

    // Parámetros de explosión / daño del proyectil (defaults)
    int danoBase;
    float radioExpl;

    bool moviendoIzquierda = false;
    bool moviendoDerecha = false;
    bool disparando = false;


};
#endif // CANON_H
