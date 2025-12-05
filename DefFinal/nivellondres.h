#ifndef NIVELLONDRES_H
#define NIVELLONDRES_H

#include "Nivel.h"
#include "Soldado.h"
#include "AvionEnemigo.h"
#include "MotorAprendizaje.h"
#include <QQueue>
#include <QMap>
#include <QPixmap>

class NivelLondres : public Nivel {
    Q_OBJECT

public:
    explicit NivelLondres(QObject* parent = nullptr);
    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;
    EntidadJuego* getJugador() const override { return soldadoJugador; }
    void setSpriteCache(const QMap<QString, QPixmap>& cache) override { spriteCache = cache; }
    int getExplosionesRecibidas() const;
    void resetearExplosiones();

private slots:
    void registrarPosicionJugador(const QVector2D& pos);

private:
    Soldado* soldadoJugador;
    AvionEnemigo* bombarderoEnemigo;
    int explosionesRecibidas;
    const int maxExplosionesPermitidas = 3;
    QMap<QString, QPixmap> spriteCache;
    float tiempoDisparo;
    float tiempoIA;
    QQueue<QVector2D> historialJugador;
    MotorAprendizaje motorIA;
    const float intervaloIA = 0.5f;
};

#endif
