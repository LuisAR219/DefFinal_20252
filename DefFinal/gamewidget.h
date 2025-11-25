#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QVector2D>
#include "NivelLondres.h"

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onUpdate();

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    NivelLondres* nivel;
    QTimer updateTimer;
    QVector2D direccionInput;
    QTimer nivelTimer;

    QMap<QString, QPixmap> spriteCache;
    void cargarSprites();
    QString obtenerSpriteParaEntidad(EntidadJuego* e);
};

#endif // GAMEWIDGET_H
