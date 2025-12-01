#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "canon.h"
#include "barco.h"
#include "proyectilbalistico.h"
#include <QVector>
#include <QPainter>
#include <QRandomGenerator>

class Nivel2 : public Nivel {
    Q_OBJECT

public:
    explicit Nivel2(QObject* parent = nullptr);
    ~Nivel2() override;

    void inicializar() override;
    void actualizar(float dt) override;
    bool nivelCompletado() const override;
    void pintar(QPainter* painter);
    void procesarTeclaPress();   // <─ agregado
    void procesarTeclaRelease(); // <─ agregado
    Canon* obtenerCanon();

    int getBarcosDerrotados() const { return barcosDerrotados; }
    int getTotalBarcos() const { return totalBarcosPorDerrotar; }

private:
    Canon canon;
    QVector<EntidadJuego*> entidadesNivel; // <─ ahora sí existe
    int totalBarcosPorDerrotar = 0;
    int barcosDerrotados = 0;
    int barcosGenerados = 0;
    int maxSimultaneos = 2;
    float tiempoDesdeUltimoSpawn = 0.0f;
    float intervaloSpawnMin = 1.0f;
    float intervaloSpawnMax = 3.0f;
    float tiempoProximoSpawn = 0.0f;

    void generarSiguienteBarco();
    int contarBarcosActivos() const;
    void verificarColisiones();
    QVector<EntidadJuego*> recogerEntidadesAEliminar();
    void eliminarEntidad(EntidadJuego* entidad);
};

#endif // NIVEL2_H
