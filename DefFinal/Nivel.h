#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include "EntidadJuego.h"
#include <QMap>
#include <QPixmap>

class Nivel : public QObject {
    Q_OBJECT

public:
    explicit Nivel(QObject* parent = nullptr);
    virtual void inicializar() = 0;
    virtual void actualizar(float dt) = 0;
    virtual bool nivelCompletado() const = 0;
    virtual EntidadJuego* getJugador() const { return nullptr; } // AÑADIDO const
    virtual void setSpriteCache(const QMap<QString, QPixmap>& cache) { Q_UNUSED(cache); }
    const QVector<EntidadJuego*>& getEntidades() const { return entidades; }

signals:
    void nivelFallido();
    void nivelCompletado();

public slots:
    virtual void eliminarEntidad(EntidadJuego* entidad) {
        if (!entidad) return;
        int idx = entidades.indexOf(entidad);
        if (idx != -1) entidades.removeAt(idx);
        delete entidad;
    }

protected:
    QVector<EntidadJuego*> entidades;
    QTimer temporizadorNivel;
    bool completado;

private slots:
    void onUpdate() { actualizar(0.016f); }
};

#endif
