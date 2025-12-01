#ifndef ENTIDADMOVIL_H
#define ENTIDADMOVIL_H

#include "EntidadJuego.h"
#include <QVector2D>

class EntidadMovil : public EntidadJuego {
    Q_OBJECT

public:
    explicit EntidadMovil(QObject* parent = nullptr);

    virtual void aplicarFisica(float dt);
    void aplicarFuerza(const QVector2D& fuerza) override;

    float getMasa() const;
    void setMasa(float m);

    QVector2D getAceleracion() const;
    void setAceleracion(const QVector2D& a);

protected:
    float masa;
    QVector2D aceleracion;
};

#endif // ENTIDADMOVIL_H
