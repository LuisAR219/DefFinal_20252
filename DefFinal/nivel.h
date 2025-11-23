#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include "entidadjuego.h"


class Nivel : public QObject {
    Q_OBJECT

public:
    explicit Nivel(QObject* parent = nullptr);

    virtual void inicializar() = 0;

    virtual void actualizar(float dt) = 0;

    virtual bool nivelCompletado() const = 0;

    const QVector<EntidadJuego*>& getEntidades() const { return entidades; }

protected:
    QVector<EntidadJuego*> entidades;
    QTimer temporizadorNivel;
    bool completado;

protected slots:
    void onUpdate();
};

#endif // NIVEL_H
