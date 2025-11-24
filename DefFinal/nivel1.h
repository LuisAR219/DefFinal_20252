#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "tanquejugador.h"
#include "controlspawn.h"
#include "entidadfija.h"
#include "enemigo.h"
#include "proyectil.h"

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

private slots:
    void onEnemigoGenerado(Enemigo* nuevo);
    void onDisparoGenerado(EntidadJuego* proyectil);

private:
    TanqueJugador* jugador;
    ControlSpawn* gestorSpawn;
    float tiempoNivel;

    float distanciaObjetivo;

    // Spawn management
    int maxEnemigosActivos;
    float tiempoDesdeUltimoSpawn;
    float intervaloSpawn; // tiempo mínimo entre spawns cuando se necesita mantener max

    // Lógica interna
    void verificarColisiones();
    void procesarColisionJugador(EntidadJuego* entidad);
    void eliminarEntidad(EntidadJuego* entidad);

    QVector<EntidadJuego*> recogerEntidadesAEliminar();

    int contarEnemigosActivos() const;
};

#endif // NIVEL1_H

