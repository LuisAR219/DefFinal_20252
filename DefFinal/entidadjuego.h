#ifndef ENTIDADJUEGO_H
#define ENTIDADJUEGO_H

#include <QObject>
#include <QVector2D>

enum TipoEntidad {
    JUGADOR,
    ENEMIGO,
    NEUTRAL
};

class EntidadJuego : public QObject {
    Q_OBJECT

public:
    explicit EntidadJuego(QObject* parent = nullptr);

    virtual void actualizar(float dt) = 0;
    virtual void aplicarFuerza(const QVector2D& fuerza) = 0;
    virtual bool colisionaCon(const EntidadJuego* otra) const = 0;

};

#endif // ENTIDADJUEGO_H
