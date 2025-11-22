#ifndef ENTIDADJUEGO_H
#define ENTIDADJUEGO_H

#include <QObject>
#include <QVector2D>

class QPainter;

enum TipoEntidad {
    JUGADOR,
    ENEMIGO,
    NEUTRAL
};

class EntidadJuego : public QObject {
    Q_OBJECT

public:
    // Constructor: posición por defecto (0,0), masa por defecto 1.0, radio por defecto 10.0
    explicit EntidadJuego(QObject* parent = nullptr,
                          const QVector2D& posicionInicial = QVector2D(0.0f, 0.0f),
                          float masaInicial = 1.0f,
                          float radioColisionInicial = 10.0f,
                          TipoEntidad tipoInicial = NEUTRAL);

    // Destructor para herencia
    virtual ~EntidadJuego();

    // Interfaz que deben implementar las subclases
    virtual void actualizar(float dt) = 0;
    virtual void aplicarFuerza(const QVector2D& fuerza) = 0;
    virtual bool colisionaCon(const EntidadJuego* otra) const = 0;
    virtual void pintar(QPainter* pintor) = 0;

    // Getters / Setters
    QVector2D obtenerPosicion() const;
    void establecerPosicion(const QVector2D& pos);

    QVector2D obtenerVelocidad() const;
    void establecerVelocidad(const QVector2D& vel);

    float obtenerRadioColision() const;
    void establecerRadioColision(float radio);

    float obtenerMasa() const;
    void establecerMasa(float m);

    TipoEntidad obtenerTipo() const;
    void establecerTipo(TipoEntidad t);

protected:
    // Estado físico básico (protegido para acceso en subclases)
    QVector2D posicion;
    QVector2D velocidad;
    float masa;
    float radioColision;
    TipoEntidad tipoEntidad;
};

#endif // ENTIDADJUEGO_H

