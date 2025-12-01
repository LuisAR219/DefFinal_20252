#include "canon.h"
#include <QtMath>
#include <QDebug>

Canon::Canon()
    : posicionBase(120.0f, 160.0f),
    minAltura(80.0f),
    maxAltura(320.0f),
    tiempoAcumulado(0.0f),
    amplitud(120.0f),
    periodo(3.0f),
    alturaActual(160.0f),
    anguloActual(0.4f),
    anguloMin(0.0f),
    anguloMax(M_PI/2.0f),
    anguloPeriodo(2.4f),
    potenciaNorm(0.0f),
    potenciaDireccional(1.0f),
    potenciaPeriodo(1.2f),
    estado(0),
    teclaPresionada(false),
    listoParaDisparo(false),
    velocidadMin(150.0f),
    velocidadMax(900.0f),
    danoBase(40),
    radioExpl(40.0f),
    moviendoIzquierda(false),
    moviendoDerecha(false),
    disparando(false)
{
}

void Canon::inicializar(const QVector2D& posBase, float alturaMin, float alturaMax)
{
    posicionBase = posBase;
    minAltura = alturaMin;
    maxAltura = alturaMax;
    amplitud = (maxAltura - minAltura) / 2.0f;
    alturaActual = minAltura + amplitud;
}

void Canon::actualizar(float dt)
{
    tiempoAcumulado += dt;

    const float velocidadAngularManual = 1.2f;

    if (estado == 0) {
        // Oscilación vertical (seno)
        float omega = 2.0f * M_PI / periodo;
        float centro = minAltura + amplitud;
        alturaActual = centro + amplitud * qSin(omega * tiempoAcumulado);
    }
    else if (estado == 1) {
        // Ángulo oscilante entre anguloMin y anguloMax
        float omegaA = 2.0f * M_PI / anguloPeriodo;
        float medio = (anguloMin + anguloMax) / 2.0f;
        float semiamplitud = (anguloMax - anguloMin) / 2.0f;
        // Si el jugador NO está moviendo manualmente, dejamos la oscilación
        if (!moviendoIzquierda && !moviendoDerecha) {
            anguloActual = medio + semiamplitud * qSin(omegaA * tiempoAcumulado);
        } else {
            // Ajuste manual: A -> disminuir ángulo (apuntar más abajo), D -> aumentar (apuntar más arriba)
            float delta = 0.0f;
            if (moviendoIzquierda) delta -= velocidadAngularManual * dt;
            if (moviendoDerecha)  delta += velocidadAngularManual * dt;
            anguloActual = qBound(anguloMin, anguloActual + delta, anguloMax);
        }
    }
    else if (estado == 2) {
        float omegaP = 2.0f * M_PI / potenciaPeriodo;
        potenciaNorm = 0.5f + 0.5f * qSin(omegaP * tiempoAcumulado);
    }
}

void Canon::pintar(QPainter* painter)
{
    // Dibujar base del cañón (un rectángulo) y la boca rotada por anguloActual
    float xBase = posicionBase.x();
    float yBase = alturaActual;

    // Cuerpo
    painter->setBrush(QColor(80,80,80));
    painter->setPen(Qt::NoPen);
    painter->drawRect(xBase - 16, yBase - 12, 32, 24);

    // Boca - se dibuja como un rectángulo rotado
    painter->save();
    painter->translate(xBase, yBase);
    float angDeg = -anguloActual * 180.0f / M_PI; // signo para coordenadas de pantalla
    painter->rotate(angDeg);
    painter->setBrush(QColor(120,120,120));
    painter->drawRect(0, -6, 36, 12);
    painter->restore();

    // Barra de potencia
    if (estado == 2) {
        float bx = xBase - 8;
        float by = yBase + 40;
        float bw = 16;
        float bh = 120;
        // fondo
        painter->setBrush(Qt::darkGray);
        painter->drawRect(bx, by, bw, bh);
        // potenciómetro (barra que rellena de abajo hacia arriba)
        float llenado = potenciaNorm * bh;
        painter->setBrush(Qt::red);
        painter->drawRect(bx, by + (bh - llenado), bw, llenado);
    }
}

void Canon::procesarPress()
{
    if (teclaPresionada) return;
    teclaPresionada = true;

    if (estado == 0) {
        estado = 1;
        tiempoAcumulado = 0.0f;
    }
    else if (estado == 1) {
        estado = 2;
        tiempoAcumulado = 0.0f;
    }
    else if (estado == 2) {
    }
}

void Canon::procesarRelease()
{
    if (!teclaPresionada) return;
    teclaPresionada = false;

    if (estado == 2) {
        listoParaDisparo = true;
    }
}

ProyectilBalistico* Canon::generarProyectilSiListo()
{
    if (!listoParaDisparo) return nullptr;

    listoParaDisparo = false;
    estado = 0;
    tiempoAcumulado = 0.0f;

    float v0 = velocidadMin + potenciaNorm * (velocidadMax - velocidadMin);
    float ang = anguloActual; // en radianes

    float xSalida = posicionBase.x() + 36.0f * qCos(ang);
    float ySalida = alturaActual - 36.0f * qSin(ang);

    ProyectilBalistico* p = new ProyectilBalistico(nullptr,
                                                   QVector2D(xSalida, ySalida),
                                                   v0,
                                                   ang,
                                                   980.0f, // gravedad por defecto
                                                   danoBase,
                                                   radioExpl,
                                                   6.0f); // radio colisión

    potenciaNorm = 0.0f;
    return p;
}

QVector2D Canon::obtenerPosicionBoca() const
{
    float xBase = posicionBase.x();
    float yBase = alturaActual;
    float ang = anguloActual;
    float xSalida = xBase + 36.0f * qCos(ang);
    float ySalida = yBase - 36.0f * qSin(ang);
    return QVector2D(xSalida, ySalida);
}

float Canon::obtenerAnguloRad() const { return anguloActual; }
float Canon::obtenerPotenciaNormalizada() const { return potenciaNorm; }

void Canon::procesarTecla(int tecla, bool presionada)
{
    switch (tecla)
    {
    case Qt::Key_A:
        moviendoIzquierda = presionada;
        break;

    case Qt::Key_D:
        moviendoDerecha = presionada;
        break;

    case Qt::Key_Space:
        if (presionada) procesarPress();
        else procesarRelease();
        break;

    default:
        break;
    }
}
