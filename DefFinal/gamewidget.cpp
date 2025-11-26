#include "GameWidget.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMessageBox>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    nivel(new NivelLondres(this)),
    soldadoJugador(nullptr),
    direccionInput(0, 0),
    frameIndex(0),
    frameCounter(0),
    bombarderoX(0),
    bombarderoDerecha(true),
    tiempoRestante(10),
    vidaMax(3),
    vidaActual(vidaMax),
    tiempoHUD(nullptr),
    explosionesHUD(nullptr),
    estadoHUD(nullptr),
    vidaBarra(nullptr),
    botonReiniciar(nullptr)
{
    qDebug() << "=== INICIANDO GameWidget ===";

    if (!nivel) {
        qFatal("ERROR: No se pudo crear NivelLondres");
        return;
    }

    nivel->inicializar();
    soldadoJugador = nivel->getJugador();

    if (!soldadoJugador) {
        qCritical() << "ERROR CRÍTICO: soldadoJugador es nullptr!";
        QMessageBox::critical(this, "Error Fatal", "No se pudo crear el jugador.");
        return;
    }

    view->setScene(scene);
    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);

    cargarSprites();
    crearHUD();

    botonReiniciar = new QPushButton("Reiniciar", this);
    botonReiniciar->setGeometry(650, 20, 100, 30);
    botonReiniciar->hide();

    connect(botonReiniciar, &QPushButton::clicked, this, &GameWidget::reiniciarNivel);
    connect(&updateTimer, &QTimer::timeout, this, &GameWidget::onUpdate);
    connect(&nivelTimer, &QTimer::timeout, this, &GameWidget::actualizarTiempo);

    updateTimer.start(16);
    nivelTimer.start(1000);

    connect(nivel, &NivelLondres::nivelFallido, this, &GameWidget::mostrarDerrota);

    setFocusPolicy(Qt::StrongFocus);

    nivel->setSpriteCache(spriteCache);

    qDebug() << "GameWidget inicializado correctamente.";
    qDebug() << "Jugador vida:" << soldadoJugador->getVida();
    qDebug() << "Nivel entidades:" << nivel->getEntidades().size();
}

GameWidget::~GameWidget() {
    if (scene) {
        QList<QGraphicsItem*> items = scene->items();
        for (QGraphicsItem* item : items) {
            scene->removeItem(item);
            delete item;
        }
    }
}

void GameWidget::crearHUD() {
    if (!scene) {
        qCritical() << "crearHUD: scene es nullptr!";
        return;
    }

    if (tiempoHUD) { scene->removeItem(tiempoHUD); delete tiempoHUD; }
    if (explosionesHUD) { scene->removeItem(explosionesHUD); delete explosionesHUD; }
    if (estadoHUD) { scene->removeItem(estadoHUD); delete estadoHUD; }
    if (vidaBarra) { scene->removeItem(vidaBarra); delete vidaBarra; }

    tiempoHUD = new QGraphicsSimpleTextItem("Tiempo: 10");
    tiempoHUD->setBrush(Qt::yellow);
    tiempoHUD->setFont(QFont("Arial", 16));
    tiempoHUD->setPos(10, 10);
    scene->addItem(tiempoHUD);

    explosionesHUD = new QGraphicsSimpleTextItem("Explosiones: 0");
    explosionesHUD->setBrush(Qt::red);
    explosionesHUD->setFont(QFont("Arial", 16));
    explosionesHUD->setPos(10, 30);
    scene->addItem(explosionesHUD);

    estadoHUD = new QGraphicsSimpleTextItem("Estado: Vivo");
    estadoHUD->setBrush(Qt::green);
    estadoHUD->setFont(QFont("Arial", 16));
    estadoHUD->setPos(10, 50);
    scene->addItem(estadoHUD);

    vidaBarra = new QGraphicsRectItem(QRectF(10, 70, 90, 15));
    vidaBarra->setPen(QPen(Qt::black));
    vidaBarra->setBrush(QBrush(Qt::green));
    scene->addItem(vidaBarra);
}

void GameWidget::cargarSprites() {
    qDebug() << "\n=== VERIFICACIÓN DE SPRITES EN TIEMPO REAL ===";

    const QString basePath = ":/sprites/imagenes/";

    fondoImagen = QPixmap(basePath + "fondo.png");
    qDebug() << "fondo.png -> Tamaño:" << fondoImagen.size() << "Cargado:" << !fondoImagen.isNull();

    spriteCache["bombardero"] = QPixmap(basePath + "Avion.png");
    spriteCache["bomba"] = QPixmap(basePath + "Bala.png");
    spriteCache["explosion"] = QPixmap(basePath + "Fuego.png");
    spriteCache["soldado"] = QPixmap(basePath + "Parado1.png");

    framesCorriendo.clear();
    for (int i = 1; i <= 6; ++i) {
        QString path = basePath + QString("Corriendo%1.png").arg(i);
        QPixmap frame(path);
        framesCorriendo.append(frame);
    }
    frameParado = QPixmap(basePath + "Parado1.png");

    qDebug() << "=== FIN VERIFICACIÓN ===\n";
}

QString GameWidget::obtenerSpriteParaEntidad(EntidadJuego* e) {
    if (!e) return "bomba";

    if (dynamic_cast<Proyectil*>(e)) return "bomba";
    if (dynamic_cast<AvionEnemigo*>(e)) return "bombardero";
    if (e == soldadoJugador) return "soldado";

    return "explosion";
}

QSize GameWidget::obtenerTamañoSprite(EntidadJuego* e) {
    QString key = obtenerSpriteParaEntidad(e);
    if (!spriteCache.contains(key)) return QSize(30, 30);

    QPixmap original = spriteCache[key];
    if (key == "bomba") {
        return original.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation).size();
    }
    return original.size();
}

QPixmap GameWidget::spriteJugador() {
    if (framesCorriendo.isEmpty()) return QPixmap();
    if (frameParado.isNull()) return QPixmap();

    if (direccionInput.isNull()) return frameParado;

    frameCounter++;
    if (frameCounter >= 5) {
        frameIndex = (frameIndex + 1) % framesCorriendo.size();
        frameCounter = 0;
    }
    return framesCorriendo[frameIndex];
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    if (!soldadoJugador) return;

    switch (event->key()) {
    case Qt::Key_W: direccionInput.setY(-1); break;
    case Qt::Key_S: direccionInput.setY(1); break;
    case Qt::Key_A: direccionInput.setX(-1); break;
    case Qt::Key_D: direccionInput.setX(1); break;
    default: return;
    }

    soldadoJugador->recibirInput(direccionInput);
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_S: direccionInput.setY(0); break;
    case Qt::Key_A:
    case Qt::Key_D: direccionInput.setX(0); break;
    default: return;
    }

    if (soldadoJugador) soldadoJugador->recibirInput(direccionInput);
}

void GameWidget::onUpdate() {
    if (!scene || !nivel || !soldadoJugador) return;

    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (item != tiempoHUD && item != explosionesHUD &&
            item != estadoHUD && item != vidaBarra) {
            scene->removeItem(item);
            delete item;
        }
    }

    if (!fondoImagen.isNull()) {
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(
            fondoImagen.scaled(scene->sceneRect().size().toSize(),
                               Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation));
        fondoItem->setZValue(-100);
        scene->addItem(fondoItem);
    }

    const auto& entidades = nivel->getEntidades();
    for (EntidadJuego* entidad : entidades) {
        if (!entidad) continue;

        if (entidad == soldadoJugador) {
            QPixmap spriteJugadorActual = spriteJugador();
            if (!spriteJugadorActual.isNull()) {
                QGraphicsPixmapItem* jugadorItem = new QGraphicsPixmapItem(spriteJugadorActual);
                float offsetY = 40;
                jugadorItem->setPos(entidad->getPosicion().x(),
                                    entidad->getPosicion().y() - offsetY);
                jugadorItem->setZValue(10);
                scene->addItem(jugadorItem);
            }
            continue;
        }

        QString spriteKey = obtenerSpriteParaEntidad(entidad);
        QPixmap spriteOriginal = spriteCache.value(spriteKey);

        if (!spriteOriginal.isNull()) {
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem();

            if (spriteKey == "bomba") {
                QPixmap bombaEscalada = spriteOriginal.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                item->setPixmap(bombaEscalada);
            } else {
                item->setPixmap(spriteOriginal);
            }

            if (spriteKey == "bombardero") {
                item->setPos(entidad->getPosicion().x(), 0);
            } else {
                item->setPos(entidad->getPosicion().x(), entidad->getPosicion().y());
            }

            item->setZValue(5);
            scene->addItem(item);
        }
    }

    actualizarHUD();
}

void GameWidget::actualizarHUD() {
    if (tiempoHUD) tiempoHUD->setText(QString("Tiempo: %1").arg(tiempoRestante));
    if (explosionesHUD) explosionesHUD->setText(
            QString("Explosiones: %1").arg(nivel->getExplosionesRecibidas())
            );
    if (estadoHUD && soldadoJugador) {
        QString estado = soldadoJugador->getVida() > 0 ? "Vivo" : "Muerto";
        estadoHUD->setText(QString("Estado: %1").arg(estado));
    }
    if (vidaBarra && soldadoJugador) {
        float vidaPercent = soldadoJugador->getVida() / 100.0f;
        int width = qBound(0, static_cast<int>(90 * vidaPercent), 90);
        vidaBarra->setRect(10, 70, width, 15);

        QColor color = vidaPercent > 0.5 ? Qt::green :
                           vidaPercent > 0.25 ? Qt::yellow : Qt::red;
        vidaBarra->setBrush(QBrush(color));
    }
}

void GameWidget::actualizarTiempo() {
    if (tiempoRestante > 0) {
        tiempoRestante--;
        actualizarHUD();
    } else {
        mostrarVictoria();
    }
}

void GameWidget::mostrarDerrota() {
    qDebug() << "=== MOSTRANDO DERROTA ===";
    if (estadoHUD) estadoHUD->setText("Estado: Derrota");

    QGraphicsSimpleTextItem* texto = new QGraphicsSimpleTextItem("¡Has perdido!");
    texto->setBrush(Qt::red);
    texto->setFont(QFont("Arial", 24));
    texto->setPos(250, 250);
    scene->addItem(texto);

    if (botonReiniciar) botonReiniciar->show();
    updateTimer.stop();
    nivelTimer.stop();
}

void GameWidget::mostrarVictoria() {
    qDebug() << "=== MOSTRANDO VICTORIA ===";
    if (estadoHUD) estadoHUD->setText("Estado: Victoria");

    QGraphicsSimpleTextItem* texto = new QGraphicsSimpleTextItem("¡Nivel completado!");
    texto->setBrush(Qt::white);
    texto->setFont(QFont("Arial", 24));
    texto->setPos(250, 250);
    scene->addItem(texto);

    if (botonReiniciar) botonReiniciar->show();
    updateTimer.stop();
    nivelTimer.stop();
}

void GameWidget::reiniciarNivel() {
    qDebug() << "=== REINICIANDO NIVEL ===";

    updateTimer.stop();
    nivelTimer.stop();

    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (item != tiempoHUD && item != explosionesHUD &&
            item != estadoHUD && item != vidaBarra) {
            scene->removeItem(item);
            delete item;
        }
    }

    if (botonReiniciar) botonReiniciar->hide();

    if (nivel) {
        disconnect(nivel, nullptr, this, nullptr);
        delete nivel;
        nivel = nullptr;
    }

    nivel = new NivelLondres(this);
    nivel->inicializar();
    nivel->resetearExplosiones();
    soldadoJugador = nivel->getJugador();

    if (!soldadoJugador) {
        qCritical() << "ERROR: No se pudo crear jugador al reiniciar!";
        return;
    }

    connect(nivel, &NivelLondres::nivelFallido, this, &GameWidget::mostrarDerrota);

    direccionInput = QVector2D(0, 0);
    frameIndex = 0;
    frameCounter = 0;
    bombarderoX = 0;
    bombarderoDerecha = true;
    tiempoRestante = 10;
    vidaActual = vidaMax;

    if (tiempoHUD) tiempoHUD->setText("Tiempo: 10");
    if (explosionesHUD) explosionesHUD->setText("Explosiones: 0");
    if (estadoHUD) estadoHUD->setText("Estado: Vivo");
    if (vidaBarra) vidaBarra->setRect(10, 70, 90, 15);

    nivel->setSpriteCache(spriteCache);

    updateTimer.start(16);
    nivelTimer.start(1000);

    qDebug() << "Nivel reiniciado y explosiones reseteadas.";
}

