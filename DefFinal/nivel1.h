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

class Nivel1 : public Nivel {
    Q_OBJECT

public:
    explicit Nivel1(QObject* parent = nullptr);
    ~Nivel1() override;

    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override { return completado; }
    EntidadJuego* getJugador() const { return jugador; }
    void setSpriteCache(const QMap<QString, QPixmap>& cache) override { spriteCache = cache; }
    float obtenerDistanciaObjetivo() const { return distanciaObjetivo; }
    int contarEnemigosActivos() const;

public slots:
    void eliminarEntidad(EntidadJuego* entidad) override;

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
    QMap<QString, QPixmap> spriteCache;

    void verificarColisiones();
    QVector<EntidadJuego*> recogerEntidadesAEliminar();
};

#endif // NIVEL1_H
