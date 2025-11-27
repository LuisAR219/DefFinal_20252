#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "canonjugador.h"
#include "proyectilcanon.h"
#include "barcoenemigo.h"
#include "barras_potencia.h"

#include <QVector>

class Nivel2 : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel2(QObject* parent = nullptr);
    ~Nivel2();

    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;

    QVector<EntidadJuego*>& getEntidades() override { return entidades; }

private:
    enum EstadoDisparo {
        Fase_MovVertical,
        Fase_FijarAltura,
        Fase_OscilarAngulo,
        Fase_FijarAngulo,
        Fase_OscilarPotencia,
        Fase_FijarPotencia,
        Fase_ProyectilEnVuelo
    };

    EstadoDisparo estadoActual;

    CanonJugador* canon;
    BarraPotencia* barraPotencia;
    ProyectilCanon* proyectilActual;

    QVector<EntidadJuego*> entidades;

    QVector<BarcoEnemigo*> barcosActivos;
    int totalBarcosPorDestruir;
    int barcosRestantes;

    float tiempo;

    void gestionarFases(float dt);
    void gestionarBarcos(float dt);
    void verificarColisiones();
};

#endif

