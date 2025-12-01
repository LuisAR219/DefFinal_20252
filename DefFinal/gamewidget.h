#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QVector2D>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>
#include <QPushButton>
#include "Nivel.h"

// Forward declarations
class Soldado;
class TanqueJugador;

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget();
    void resizeEvent(QResizeEvent *event) override;
    void inicializarNivel(int numeroNivel);

signals:
    void nivelCompletado();
    void nivelFallido();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onUpdate();
    void reiniciarNivel();
    void mostrarDerrota();
    void mostrarVictoria();
    void actualizarTiempo();

private:
    void cargarSprites();
    void crearHUD();
    void actualizarHUD();
    QString obtenerSpriteKey(EntidadJuego* entidad);
    QPixmap spriteJugador();
    QRectF calcularRectanguloSprite(EntidadJuego* entidad, const QString& spriteKey);
    QString obtenerSpriteParaEntidad(EntidadJuego* e);
    QSize obtenerTamañoSprite(EntidadJuego* e);

    QGraphicsScene* scene;
    QGraphicsView* view;
    Nivel* nivel;
    EntidadJuego* jugador;
    QTimer updateTimer;
    QTimer nivelTimer;
    QVector2D direccionInput;
    QPixmap fondoImagen;
    QPixmap fondoImagenLondres;
    int nivelActual;

    int frameIndex;
    int frameCounter;
    QVector<QPixmap> framesCorriendo;
    QPixmap frameParado;

    QGraphicsSimpleTextItem* tiempoHUD;
    QGraphicsSimpleTextItem* explosionesHUD;
    QGraphicsSimpleTextItem* estadoHUD;
    QGraphicsRectItem* vidaBarra;

    int vidaMax;
    int vidaActual;
    int tiempoRestante;
    QPushButton* botonReiniciar;
    QMap<QString, QPixmap> spriteCache;
};

#endif
