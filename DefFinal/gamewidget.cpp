#include "GameWidget.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include "AvionEnemigo.h"
#include "Proyectil.h"

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    direccionInput(0, 0)
{
    nivel = new NivelLondres(this);
    nivel->inicializar();

    view->setScene(scene);
    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);

    cargarSprites();

    connect(&updateTimer, &QTimer::timeout, this, &GameWidget::onUpdate);
    updateTimer.start(16);

    connect(&nivelTimer, &QTimer::timeout, this, [this]() {
        updateTimer.stop();
        QGraphicsTextItem* texto = scene->addText("¡Nivel completado!");
        texto->setDefaultTextColor(Qt::white);
        texto->setFont(QFont("Arial", 24));
        texto->setPos(250, 250);
        qDebug() << "Nivel terminado: 10 segundos cumplidos.";
    });
    nivelTimer.start(10000);

    setFocusPolicy(Qt::StrongFocus);
}

void GameWidget::cargarSprites() {
    spriteCache["soldado"] = QPixmap(":/sprites/Parado1.png");
    spriteCache["bombardero"] = QPixmap(":/sprites/Avión.png");
    spriteCache["bomba"] = QPixmap(":/sprites/Bala.png");
    spriteCache["tanque"] = QPixmap(":/sprites/Fuego.png");

    for (const QString& key : spriteCache.keys()) {
        if (spriteCache[key].isNull()) {
            qWarning() << "No se pudo cargar el sprite:" << key;
        }
    }
}

QString GameWidget::obtenerSpriteParaEntidad(EntidadJuego* e) {
    switch (e->getTipoEntidad()) {
    case JUGADOR:
        return "soldado";
    case ENEMIGO:
        if (dynamic_cast<AvionEnemigo*>(e)) return "bombardero";
        if (dynamic_cast<Proyectil*>(e)) return "bomba";
        return "bomba";
    case NEUTRAL:
        return "tanque";
    default:
        return "bomba";
    }
}

void GameWidget::onUpdate() {
    if (nivel && nivel->getJugador()) {
        nivel->getJugador()->recibirInput(direccionInput);
    }

    nivel->actualizar(0.016f);
    scene->clear();

    for (EntidadJuego* e : nivel->getEntidades()) {
        QString clave = obtenerSpriteParaEntidad(e);
        QPixmap pixmap = spriteCache.value(clave);

        if (!pixmap.isNull()) {
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
            item->setPos(e->getPosicion().x(), e->getPosicion().y());
            scene->addItem(item);
        }
    }
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_W: direccionInput.setY(-1); break;
    case Qt::Key_S: direccionInput.setY(1); break;
    case Qt::Key_A: direccionInput.setX(-1); break;
    case Qt::Key_D: direccionInput.setX(1); break;
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_S: direccionInput.setY(0); break;
    case Qt::Key_A:
    case Qt::Key_D: direccionInput.setX(0); break;
    }
}

