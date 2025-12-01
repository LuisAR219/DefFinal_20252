#ifndef MOTORAPRENDIZAJE_H
#define MOTORAPRENDIZAJE_H

#include <QQueue>
#include <QVector2D>

class MotorAprendizaje {
public:
    MotorAprendizaje();

    void analizarMovimiento(const QQueue<QVector2D>& historialJugador);
    QVector2D getDireccionPredicha() const;

private:
    QVector2D direccionPredicha;
};

#endif // MOTORAPRENDIZAJE_H
