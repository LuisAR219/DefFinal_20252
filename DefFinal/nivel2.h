#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "canon.h"
#include "barco.h"
#include "proyectilbalistico.h"
#include <QVector>
#include <QPainter>
#include <QRandomGenerator>

class Nivel2 : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel2(QObject* parent = nullptr);
    ~Nivel2() override;

    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;
    void pintar(QPainter* painter);

    void procesarTeclaPress();
    void procesarTeclaRelease();
    Canon* obtenerCanon();

private slots:

private:
    Canon canon;
    QVector<EntidadJuego*> entidadesNivel;

    int totalBarcosPorDerrotar;
    int barcosDerrotados;
    int barcosGenerados;
    int maxSimultaneos;

    float tiempoDesdeUltimoSpawn;
    float intervaloSpawnMin;
    float intervaloSpawnMax;
    float tiempoProximoSpawn;

    // Auxiliares
    void generarSiguienteBarco();
    int contarBarcosActivos() const;
    void verificarColisiones();
    QVector<EntidadJuego*> recogerEntidadesAEliminar();
    void eliminarEntidad(EntidadJuego* entidad);
};

#endif // NIVEL2_H
