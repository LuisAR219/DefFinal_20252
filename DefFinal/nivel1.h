#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "tanquejugador.h"
#include "controlspawn.h"
#include "entidadfija.h"
#include "enemigo.h"
#include "proyectil.h"
#include "obstaculo.h"

#include <QVector>

class Nivel1 : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel1(QObject* parent = nullptr);
    ~Nivel1();

    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;
    float obtenerDistanciaObjetivo() const { return distanciaObjetivo; }

private slots:
    void onEnemigoGenerado(Enemigo* nuevo);
    void onDisparoGenerado(EntidadJuego* proyectil);

private:
    TanqueJugador* jugador;
    ControlSpawn* gestorSpawn;
    float tiempoNivel;

    float distanciaObjetivo;

    int maxEnemigosActivos;
    float tiempoDesdeUltimoSpawn;
    float intervaloSpawn;

    float tiempoDesdeUltimoObstaculo;
    float intervaloObstaculos;

    void verificarColisiones();
    void procesarColisionJugador(EntidadJuego* entidad);
    void eliminarEntidad(EntidadJuego* entidad);

    QVector<EntidadJuego*> recogerEntidadesAEliminar();

    int contarEnemigosActivos() const;
};

#endif // NIVEL1_H
