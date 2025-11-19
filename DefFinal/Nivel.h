#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include "EntidadJuego.h"

class Nivel : public QObject {
    Q_OBJECT

public:
    explicit Nivel(QObject* parent = nullptr);

    virtual void inicializar() = 0;

    virtual void actualizar(float dt) = 0;

};

#endif // NIVEL_H
