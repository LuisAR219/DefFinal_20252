#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "tanquejugador.h"
#include "controlspawn.h"
#include "entidadfija.h"
#include "enemigo.h"

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
    // SLOT NECESARIO PARA EL SPAWN DE ENEMIGOS
    void onEnemigoGenerado(Enemigo* nuevo);

private:
    TanqueJugador* jugador;
    ControlSpawn* gestorSpawn;
    float tiempoNivel;
    float distanciaObjetivo;

    // --- Lógica interna ---
    void verificarColisiones();
    void procesarColisionJugador(EntidadJuego* entidad);
    void eliminarEntidad(EntidadJuego* entidad);

    // ESTA FUNCIÓN ERA NECESARIA Y FALTABA EN EL .H
    QVector<EntidadJuego*> recogerEntidadesAEliminar();
};

#endif // NIVEL1_H
