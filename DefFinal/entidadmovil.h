#ifndef ENTIDADMOVIL_H
#define ENTIDADMOVIL_H

#include "EntidadJuego.h"
#include <QVector2D>

class EntidadMovil : public EntidadJuego {
    Q_OBJECT

public:
    explicit EntidadMovil(QObject* parent = nullptr,
                          const QVector2D& posInicial = QVector2D(0,0),
                          float masaInicial = 1.0f,
                          float radioColisionInicial = 30.0f,
                          TipoEntidad tipoInicial = NEUTRAL);

    virtual void aplicarFisica(float dt);
    void aplicarFuerza(const QVector2D& fuerza) override;
    QVector2D getAceleracion() const { return aceleracion; }
    void setAceleracion(const QVector2D& a) { aceleracion = a; }

protected:
    QVector2D aceleracion;
};

#endif
