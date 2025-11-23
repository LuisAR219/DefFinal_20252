#include "tanquejugador.h"
#include <QtMath>
#include <QDebug>

TanqueJugador::TanqueJugador(QObject* parent,
                             const QVector2D& posicionInicial,
                             float masaInicial,
                             float radioColisionInicial)
    : EntidadJuego(parent, posicionInicial, masaInicial, radioColisionInicial, JUGADOR),
    controlVertical(nullptr),
    controlLateral(nullptr),
    vida(100.0f),
    distanciaRecorrida(0.0f),
    limiteIzquierdo(0.0f),
    limiteDerecho(800.0f),   // valores por defecto
    limiteSuperior(0.0f),
    limiteInferior(600.0f),
    velocidadMaxX(200.0f),
    velocidadMaxY(200.0f)
{
    controlVertical = new ControlTanque(this);
    controlLateral = new ControlTanque(this);

    controlVertical->establecerAceleracionMax(180.0f);
    controlVertical->establecerCoefFriccion(6.0f);
    controlVertical->establecerFactorSuavizado(0.18f);

    controlLateral->establecerAceleracionMax(180.0f);
    controlLateral->establecerCoefFriccion(6.0f);
    controlLateral->establecerFactorSuavizado(0.18f);

    // inicializamos velocidad
    velocidad = QVector2D(0.0f, 0.0f);
}

TanqueJugador::~TanqueJugador()
{
    if (controlVertical && controlVertical->parent() != this) {
        delete controlVertical;
        controlVertical = nullptr;
    }
    if (controlLateral && controlLateral->parent() != this) {
        delete controlLateral;
        controlLateral = nullptr;
    }
}

void TanqueJugador::actualizar(float dt)
{
    if (dt <= 0.0f) return;

    // 1) Obtener fuerzas por eje desde los controles
    // ControlTanque devuelve un QVector2D donde la fuerza viene en y negativa (por convención).
    QVector2D fuerzaVert = controlVertical->calcularFuerza(velocidad, masa);
    float fuerzaY = -fuerzaVert.y(); // valor escalar en eje Y (positivo = hacia arriba)

    // Para control lateral, calculamos fuerza escalar basada en la magnitud
    // Usamos la propia función del control (interpretamos su salida en magnitud)
    QVector2D velocidadSoloX(velocidad.x(), 0.0f);
    QVector2D fuerzaLat = controlLateral->calcularFuerza(velocidadSoloX, masa);
    float fuerzaX = -fuerzaLat.y(); // signo: negativo por convención interna

    // 2) Construir vector fuerza total (X,Y)
    QVector2D fuerzaTotal(fuerzaX, -fuerzaY);

    // 3) Aplicar aceleración: a = F / m
    QVector2D aceleracion = fuerzaTotal / masa;

    // 4) Integración semi-implicita
    velocidad += aceleracion * dt;

    // 5) Clampeo de velocidades
    if (qAbs(velocidad.x()) > velocidadMaxX) {
        velocidad.setX((velocidad.x() > 0.0f) ? velocidadMaxX : -velocidadMaxX);
    }
    if (qAbs(velocidad.y()) > velocidadMaxY) {
        velocidad.setY((velocidad.y() > 0.0f) ? velocidadMaxY : -velocidadMaxY);
    }

    // 6) Actualizar posición
    posicion += velocidad * dt;

    // 7) Aplicar límites (carretera/pantalla)
    aplicarLimites();

    // 8) Actualizar distancia recorrida (consideramos avance en Y negativo = hacia arriba)
    // distancia positiva incrementa cuando se avanza hacia arriba (velocidad.y < 0)
    distanciaRecorrida += qAbs(velocidad.y()) * dt;
}

void TanqueJugador::aplicarFuerza(const QVector2D& fuerza)
{
    // Aplicamos la fuerza directamente cambiando la velocidad (impulso)
    // a = F / m
    QVector2D aceleracion = fuerza / masa;
    velocidad += aceleracion;
}

bool TanqueJugador::colisionaCon(const EntidadJuego* otra) const
{
    float distancia = (otra->obtenerPosicion() - posicion).length();
    float sumaRadios = radioColision + otra->obtenerRadioColision();
    return distancia < sumaRadios;
}

void TanqueJugador::pintar(QPainter* pintor)
{
    // Centro en posicion.x(), posicion.y()
    float ancho = radioColision * 2.0f;
    float alto  = radioColision * 2.0f;

    QRectF rect(posicion.x() - ancho / 2.0f,
                posicion.y() - alto / 2.0f,
                ancho,
                alto);

    pintor->save();
    pintor->setPen(Qt::black);
    pintor->setBrush(Qt::darkGreen);
    pintor->drawRect(rect);

    // dibujar barra de vida encima del tanque
    float barraAncho = ancho;
    float barraAlto = 4.0f;
    float xBar = rect.left();
    float yBar = rect.top() - 8.0f;

    // fondo
    pintor->setBrush(Qt::red);
    pintor->drawRect(QRectF(xBar, yBar, barraAncho, barraAlto));

    // vida actual
    float porcentaje = qBound(0.0f, vida / 100.0f, 1.0f);
    pintor->setBrush(Qt::green);
    pintor->drawRect(QRectF(xBar, yBar, barraAncho * porcentaje, barraAlto));

    pintor->restore();
}

ControlTanque* TanqueJugador::obtenerControlVertical() const
{
    return controlVertical;
}

ControlTanque* TanqueJugador::obtenerControlLateral() const
{
    return controlLateral;
}

void TanqueJugador::recibirDano(int dano)
{
    vida -= static_cast<float>(dano);
    if (vida < 0.0f) vida = 0.0f;
}

float TanqueJugador::obtenerVida() const
{
    return vida;
}

void TanqueJugador::establecerVida(float valor)
{
    vida = valor;
}

float TanqueJugador::obtenerDistanciaRecorrida() const
{
    return distanciaRecorrida;
}

void TanqueJugador::establecerLimites(float izquierdo, float derecho, float superior, float inferior)
{
    limiteIzquierdo = izquierdo;
    limiteDerecho = derecho;
    limiteSuperior = superior;
    limiteInferior = inferior;
}

void TanqueJugador::establecerVelocidadMaxima(float maxX, float maxY)
{
    velocidadMaxX = maxX;
    velocidadMaxY = maxY;
}

void TanqueJugador::aplicarLimites()
{
    // Límite horizontal
    if (posicion.x() < limiteIzquierdo) {
        posicion.setX(limiteIzquierdo);
        velocidad.setX(0.0f);
    } else if (posicion.x() > limiteDerecho) {
        posicion.setX(limiteDerecho);
        velocidad.setX(0.0f);
    }

    // Límite vertical
    if (posicion.y() < limiteSuperior) {
        posicion.setY(limiteSuperior);
        velocidad.setY(0.0f);
    } else if (posicion.y() > limiteInferior) {
        posicion.setY(limiteInferior);
        velocidad.setY(0.0f);
    }
}
