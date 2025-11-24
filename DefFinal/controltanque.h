#ifndef CONTROLTANQUE_H
#define CONTROLTANQUE_H

#include <QVector2D>
#include <QObject>

class ControlTanque : public QObject
{
    Q_OBJECT

public:
    explicit ControlTanque(QObject* parent = nullptr);

    void establecerInput(bool acelera, bool frena, bool dispara = false);

    QVector2D calcularFuerza(const QVector2D& velocidadActual, float masa);

    void establecerAceleracionMax(float valor);
    void establecerCoefFriccion(float valor);
    void establecerFactorSuavizado(float valor);

private:
    bool acelerando;
    bool frenando;
    bool disparando;

    float aceleracionMax;
    float coefFriccion;
    float factorSuavizado;

    float aceleracionActual;
};

#endif // CONTROLTANQUE_H
