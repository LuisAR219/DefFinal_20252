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
#include "NivelLondres.h"
#include "Soldado.h"

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget();

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
    QString obtenerSpriteParaEntidad(EntidadJuego* e);
    QPixmap spriteJugador();
    QSize obtenerTamañoSprite(EntidadJuego* e);

    QGraphicsScene* scene;
    QGraphicsView* view;
    NivelLondres* nivel;
    QTimer updateTimer;
    QTimer nivelTimer;
    QVector2D direccionInput;
    QPixmap fondoImagen;
    Soldado* soldadoJugador;

    int frameIndex;
    int frameCounter;
    QVector<QPixmap> framesCorriendo;
    QPixmap frameParado;
    float bombarderoX;
    bool bombarderoDerecha;

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

#endif // GAMEWIDGET_H
