#include "MotorAprendizaje.h"

MotorAprendizaje::MotorAprendizaje()
    : direccionPredicha(0,0)
{}

void MotorAprendizaje::analizarMovimiento(const QQueue<QVector2D>& historialJugador) {
    if(historialJugador.isEmpty()) return;
    QVector2D ultima = historialJugador.back();
    QVector2D penultima = historialJugador.size() > 1 ? historialJugador[historialJugador.size()-2] : ultima;
    direccionPredicha = ultima - penultima;
}

QVector2D MotorAprendizaje::getDireccionPredicha() const {
    return direccionPredicha;
}
