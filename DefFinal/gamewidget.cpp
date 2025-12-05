#include "GameWidget.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QFile>
#include "Nivel1.h"
#include "Nivel2.h"
#include "NivelLondres.h"
#include "Soldado.h"
#include "TanqueJugador.h"
#include "Barco.h"
#include "Obstaculo.h"

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    nivel(nullptr),
    jugador(nullptr),
    direccionInput(0, 0),
    frameIndex(0),
    frameCounter(0),
    nivelActual(0),
    fondoImagen(),
    fondoImagenLondres(),
    tiempoRestante(60),
    vidaMax(5),
    vidaActual(vidaMax),
    tiempoHUD(nullptr),
    explosionesHUD(nullptr),
    estadoHUD(nullptr),
    vidaBarra(nullptr),
    botonReiniciar(nullptr)
{
    scene->setSceneRect(0, 0, 800, 600);

    view->setRenderHint(QPainter::Antialiasing);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setStyleSheet("background-color: black;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    cargarSprites();

    botonReiniciar = new QPushButton("Reiniciar", this);
    botonReiniciar->setGeometry(650, 20, 100, 30);
    botonReiniciar->hide();
    connect(botonReiniciar, &QPushButton::clicked, this, &GameWidget::reiniciarNivel);

    connect(&updateTimer, &QTimer::timeout, this, &GameWidget::onUpdate);
    connect(&nivelTimer, &QTimer::timeout, this, &GameWidget::actualizarTiempo);
    updateTimer.start(16);

    qDebug() << "GameWidget inicializado correctamente.";
}

GameWidget::~GameWidget() {
    if (scene) {
        QList<QGraphicsItem*> items = scene->items();
        for (QGraphicsItem* item : items) {
            scene->removeItem(item);
            delete item;
        }
    }
    if (nivel) delete nivel;
}

void GameWidget::inicializarNivel(int numeroNivel) {
    if (nivel) {
        disconnect(nivel, nullptr, this, nullptr);
        delete nivel;
        nivel = nullptr;
    }

    jugador = nullptr;

    switch(numeroNivel) {
    case 1:
        nivel = new Nivel1(this);
        tiempoRestante = 10; // Ahora 10 segundos
        vidaMax = 5;
        break;
    case 2:
        nivel = new Nivel2(this);
        tiempoRestante = 20;
        vidaMax = 1;
        break;
    case 3:
        nivel = new NivelLondres(this);
        tiempoRestante = 10;
        vidaMax = 3;
        break;
    default:
        qCritical() << "Número de nivel inválido:" << numeroNivel;
        return;
    }

    nivelActual = numeroNivel;
    vidaActual = vidaMax;

    nivel->inicializar();

    if (nivelActual == 1) {
        Nivel1* nivel1 = dynamic_cast<Nivel1*>(nivel);
        if (nivel1) jugador = nivel1->getJugador();
    } else if (nivelActual == 3) {
        NivelLondres* nivelLondres = dynamic_cast<NivelLondres*>(nivel);
        if (nivelLondres) jugador = nivelLondres->getJugador();
    }

    if (!scene) {
        qCritical() << "Scene es nullptr en inicializarNivel";
        return;
    }

    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (item != tiempoHUD && item != explosionesHUD &&
            item != estadoHUD && item != vidaBarra) {
            scene->removeItem(item);
            delete item;
        }
    }

    QPixmap* fondoActual = nullptr;
    if (nivelActual == 2) {
        fondoActual = &spriteCache["fondo2"];
    } else if (nivelActual == 3) {
        fondoActual = &fondoImagenLondres;
    } else {
        fondoActual = &fondoImagen;
    }

    if (fondoActual && !fondoActual->isNull()) {
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(
            fondoActual->scaled(scene->sceneRect().size().toSize(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation));
        fondoItem->setZValue(-100);
        scene->addItem(fondoItem);
    }

    connect(nivel, SIGNAL(nivelFallido()), this, SLOT(mostrarDerrota()));
    connect(nivel, SIGNAL(nivelCompletado()), this, SLOT(mostrarVictoria()));

    nivelTimer.start(1000);

    if (nivelActual == 1) {
        Nivel1* nivel1 = dynamic_cast<Nivel1*>(nivel);
        if (nivel1) nivel1->setSpriteCache(spriteCache);
    } else if (nivelActual == 3) {
        NivelLondres* nivelLondres = dynamic_cast<NivelLondres*>(nivel);
        if (nivelLondres) nivelLondres->setSpriteCache(spriteCache);
    }

    frameIndex = 0;
    frameCounter = 0;
    direccionInput = QVector2D(0, 0);

    if (botonReiniciar) botonReiniciar->hide();

    crearHUD();
    actualizarHUD();
}

void GameWidget::cargarSprites() {
    qDebug() << "\n=== VERIFICACIÓN DE SPRITES EN TIEMPO REAL ===";

    const QString basePath = ":/sprites/imagenes/";

    fondoImagen = QPixmap(basePath + "fondo.png");
    fondoImagenLondres = QPixmap(basePath + "fondo1.png");
    spriteCache["fondo2"] = QPixmap(basePath + "fondo2.png");

    spriteCache["bombardero"] = QPixmap(basePath + "Avion.png");
    spriteCache["barco"] = QPixmap(basePath + "barco1.png");
    spriteCache["bomba"] = QPixmap(basePath + "Bala.png");
    spriteCache["bombaLondres"] = QPixmap(basePath + "Bala1.png");
    spriteCache["explosion"] = QPixmap(basePath + "Fuego.png");
    spriteCache["soldado"] = QPixmap(basePath + "Parado1.png");

    framesCorriendo.clear();
    for (int i = 1; i <= 6; ++i) {
        QString path = basePath + QString("Corriendo%1.png").arg(i);
        QPixmap frame(path);
        framesCorriendo.append(frame);
    }
    frameParado = QPixmap(basePath + "Parado1.png");

    spriteCache["tanque"] = QPixmap(basePath + "tanque.png");
    spriteCache["tanqueEne"] = QPixmap(basePath + "tanqueEne.png");
    spriteCache["rueda"] = QPixmap(basePath + "rueda.png");
    spriteCache["barril"] = QPixmap(basePath + "barril.png");
    spriteCache["mina"] = QPixmap(basePath + "mina.png");

    qDebug() << "Sprites cargados - fondo1:" << !fondoImagenLondres.isNull()
             << "Bala1:" << !spriteCache.value("bombaLondres").isNull();
    qDebug() << "=== FIN VERIFICACIÓN ===\n";
}

QString GameWidget::obtenerSpriteKey(EntidadJuego* entidad) {
    if (!entidad) return "explosion";

    // Jugador
    if (entidad == jugador) {
        if (nivelActual == 3) return "soldado";
        if (nivelActual == 1) return "tanque";
        return "soldado";
    }

    // Proyectiles normales (Nivel 1 y 3)
    if (dynamic_cast<Proyectil*>(entidad)) {
        return (nivelActual == 3) ? "bombaLondres" : "bomba";
    }

    // Proyectiles balísticos (Nivel 2)
    if (dynamic_cast<ProyectilBalistico*>(entidad)) {
        return "bomba";
    }

    // Avión enemigo (Nivel 3)
    if (dynamic_cast<AvionEnemigo*>(entidad)) {
        return "bombardero";
    }

    // Tanques enemigos (Nivel 1)
    if (dynamic_cast<Enemigo*>(entidad)) {
        return "tanqueEne";
    }

    // Barcos (Nivel 2) - tienen su propio pintar, pero usamos placeholder
    if (dynamic_cast<Barco*>(entidad)) {
        return "explosion"; // Placeholder, renderizaremos con rectángulo
    }

    // Obstáculos (Nivel 1) - tienen su propio pintar
    if (dynamic_cast<Obstaculo*>(entidad)) {
        return "explosion"; // Placeholder, renderizaremos con rectángulo
    }

    return "explosion";
}

QRectF GameWidget::calcularRectanguloSprite(EntidadJuego* entidad, const QString& spriteKey) {
    QVector2D pos = entidad->getPosicion();
    QPixmap sprite = spriteCache[spriteKey];

    if (spriteKey == "bomba" || spriteKey == "bombaLondres") {
        return QRectF(pos.x() - 15, pos.y() - 15, 30, 30);
    }
    else if (spriteKey == "bombardero") {
        // El avión se dibuja en una posición fija Y con X variable
        return QRectF(pos.x() - sprite.width()/2, -180, sprite.width(), sprite.height());
    }
    else if (spriteKey == "tanque") {
        return QRectF(pos.x() - 18, pos.y() - 18, 36, 36); // Tamaño reducido
    }
    else if (spriteKey == "tanqueEne") {
        return QRectF(pos.x() - 18, pos.y() - 18, 36, 36); // Tamaño reducido
    }
    else if (spriteKey == "soldado") {
        return QRectF(pos.x() - 30, pos.y() - 40, 60, 80);
    }

    // Default
    return QRectF(pos.x() - 20, pos.y() - 20, 40, 40);
}

QPixmap GameWidget::spriteJugador() {
    if (framesCorriendo.isEmpty() || frameParado.isNull()) return QPixmap();

    if (direccionInput.isNull()) return frameParado;

    frameCounter++;
    if (frameCounter >= 5) {
        frameIndex = (frameIndex + 1) % framesCorriendo.size();
        frameCounter = 0;
    }
    return framesCorriendo[frameIndex];
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F11) {
        if (isFullScreen()) showNormal(); else showFullScreen();
        return;
    }

    if (nivelActual == 2) {
        Nivel2* nivel2 = dynamic_cast<Nivel2*>(nivel);
        if (nivel2) nivel2->obtenerCanon()->procesarTecla(event->key(), true);
        return;
    }

    if (!jugador) return;

    switch (event->key()) {
    case Qt::Key_W: direccionInput.setY(-1); break;
    case Qt::Key_S: direccionInput.setY(1); break;
    case Qt::Key_A: direccionInput.setX(-1); break;
    case Qt::Key_D: direccionInput.setX(1); break;
    default: QWidget::keyPressEvent(event); return;
    }

    if (nivelActual == 1) {
        TanqueJugador* tanque = dynamic_cast<TanqueJugador*>(jugador);
        if (tanque) tanque->teclaPresionada(event->key());
    } else if (nivelActual == 3) {
        Soldado* soldado = dynamic_cast<Soldado*>(jugador);
        if (soldado) soldado->recibirInput(direccionInput);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    if (nivelActual == 2) {
        Nivel2* nivel2 = dynamic_cast<Nivel2*>(nivel);
        if (nivel2) nivel2->obtenerCanon()->procesarTecla(event->key(), false);
        return;
    }

    if (!jugador) return;

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_S: direccionInput.setY(0); break;
    case Qt::Key_A:
    case Qt::Key_D: direccionInput.setX(0); break;
    default: return;
    }

    if (nivelActual == 1) {
        TanqueJugador* tanque = dynamic_cast<TanqueJugador*>(jugador);
        if (tanque) tanque->teclaLiberada(event->key());
    } else if (nivelActual == 3) {
        Soldado* soldado = dynamic_cast<Soldado*>(jugador);
        if (soldado) soldado->recibirInput(direccionInput);
    }
}

void GameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (view && scene) view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void GameWidget::onUpdate()
{
    if (!scene || !nivel) return;

    // 1. Limpiar escena excepto HUD
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (item != tiempoHUD && item != explosionesHUD &&
            item != estadoHUD && item != vidaBarra) {
            scene->removeItem(item);
            delete item;
        }
    }

    // 2. Fondos PRIMERO (más atrás que cualquier sprite)
    QPixmap* fondoActual = nullptr;
    if (nivelActual == 2) {
        fondoActual = &spriteCache["fondo2"];        // Pearl Harbor
    } else if (nivelActual == 3) {
        fondoActual = &fondoImagenLondres;           // Batalla de Londres
    } else {
        fondoActual = &fondoImagen;                  // Kursk
    }

    if (fondoActual && !fondoActual->isNull()) {
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(
            fondoActual->scaled(scene->sceneRect().size().toSize(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation));
        fondoItem->setZValue(-200);
        scene->addItem(fondoItem);
    }

    // 3. Cañón del nivel 2 (encima del fondo)
    if (nivelActual == 2) {
        Nivel2* nivel2 = qobject_cast<Nivel2*>(nivel);
        if (nivel2) {
            Canon* canon = nivel2->obtenerCanon();
            QPixmap canonPix(64, 32);
            canonPix.fill(Qt::darkGray);
            QGraphicsPixmapItem* canonItem = new QGraphicsPixmapItem(canonPix);
            QVector2D boca = canon->obtenerPosicionBoca();
            canonItem->setPos(boca.x() - 32, boca.y() - 16);
            canonItem->setZValue(10);
            scene->addItem(canonItem);
        }
    }

    // 4. Resto de entidades
    const auto& entidades = nivel->getEntidades();
    for (EntidadJuego* entidad : entidades) {
        if (!entidad) continue;

        // Jugador nivel 3 (soldado animado)
        if (entidad == jugador && nivelActual == 3) {
            QPixmap spriteJugadorActual = spriteJugador();
            if (!spriteJugadorActual.isNull()) {
                QGraphicsPixmapItem* jugadorItem = new QGraphicsPixmapItem(spriteJugadorActual);
                jugadorItem->setPos(entidad->getPosicion().x() - spriteJugadorActual.width()/2,
                                    entidad->getPosicion().y() - spriteJugadorActual.height());
                jugadorItem->setZValue(10);
                scene->addItem(jugadorItem);
            }
            continue;
        }

        QString spriteKey = obtenerSpriteKey(entidad);

        // Barcos nivel 2 → hitbox más ancha proporcional al sprite
        if (Barco* barco = dynamic_cast<Barco*>(entidad)) {
            QPixmap spriteBarco = spriteCache["barco"];
            if (!spriteBarco.isNull()) {
                QPixmap scaled = spriteBarco.scaled(240, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QGraphicsPixmapItem* item = new QGraphicsPixmapItem(scaled);
                item->setPos(barco->getPosicion().x() - scaled.width()/2,
                             barco->getPosicion().y() - scaled.height()/2);
                item->setZValue(5);
                scene->addItem(item);

                // ⬇️ Hitbox más ancha que el sprite visual
                barco->setAnchoHitbox(scaled.width() * 10.8f);
                barco->setAltoHitbox(scaled.height() * 10.2f);
            } else {
                QRectF rect(entidad->getPosicion().x() - 120, entidad->getPosicion().y() - 60, 240, 120);
                QGraphicsRectItem* item = new QGraphicsRectItem(rect);
                item->setBrush(Qt::darkCyan);
                item->setPen(QPen(Qt::red));
                item->setZValue(5);
                scene->addItem(item);
            }
            continue;
        }

        // Obstáculos nivel 1 (rectángulos placeholder)
        if (dynamic_cast<Obstaculo*>(entidad)) {
            QRectF rect(entidad->getPosicion().x() - 15, entidad->getPosicion().y() - 15, 30, 30);
            QGraphicsRectItem* item = new QGraphicsRectItem(rect);
            item->setBrush(Qt::yellow);
            item->setPen(QPen(Qt::white));
            item->setZValue(5);
            scene->addItem(item);
            continue;
        }

        // Sprites normales
        QPixmap spriteOriginal = spriteCache.value(spriteKey);
        if (spriteOriginal.isNull()) {
            qWarning() << "Sprite no encontrado:" << spriteKey;
            continue;
        }

        QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
        if (spriteKey == "bomba" || spriteKey == "bombaLondres") {
            QPixmap scaled = spriteOriginal.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            item->setPixmap(scaled);
        } else if (spriteKey == "tanque" || spriteKey == "tanqueEne") {
            QPixmap scaled = spriteOriginal.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            item->setPixmap(scaled);
        } else {
            item->setPixmap(spriteOriginal);
        }

        QRectF rect = calcularRectanguloSprite(entidad, spriteKey);
        item->setPos(rect.x(), rect.y());

        int zValue = 5;
        if (entidad == jugador) zValue = 10;
        else if (dynamic_cast<Proyectil*>(entidad) || dynamic_cast<ProyectilBalistico*>(entidad)) zValue = 8;
        else if (dynamic_cast<AvionEnemigo*>(entidad)) zValue = -5;
        else if (dynamic_cast<Enemigo*>(entidad)) zValue = 5;

        item->setZValue(zValue);
        scene->addItem(item);
    }

    // Condiciones de victoria/derrota nivel 2
    if (nivelActual == 2) {
        Nivel2* nivel2 = qobject_cast<Nivel2*>(nivel);
        if (nivel2 && nivel2->getBarcosDerrotados() >= 4) {
            emit nivelCompletado();
            return;
        }
        if (nivel2 && tiempoRestante <= 0 && nivel2->getBarcosDerrotados() < 4) {
            mostrarDerrota();
            return;
        }
    }

    actualizarHUD();
}

void GameWidget::actualizarHUD() {
    if (tiempoHUD) tiempoHUD->setText(QString("Tiempo: %1").arg(tiempoRestante));

    if (nivelActual == 1) {
        Nivel1* nivel1 = dynamic_cast<Nivel1*>(nivel);
        if (nivel1 && explosionesHUD) {
            explosionesHUD->setText(QString("Enemigos: %1").arg(nivel1->contarEnemigosActivos()));
        }
    }

    if (estadoHUD && jugador) {
        QString estado = jugador->getVida() > 0 ? "Vivo" : "Muerto";
        estadoHUD->setText(QString("Estado: %1").arg(estado));
    }

    if (vidaBarra && jugador) {
        float vidaPercent = jugador->getVida() / 100.0f;
        int width = qBound(0, static_cast<int>(90 * vidaPercent), 90);
        vidaBarra->setRect(10, 70, width, 15);

        QColor color = vidaPercent > 0.5 ? Qt::green :
                           vidaPercent > 0.25 ? Qt::yellow : Qt::red;
        vidaBarra->setBrush(QBrush(color));
    }
}

void GameWidget::actualizarTiempo() {
    if (tiempoRestante <= 0) {
        mostrarVictoria();
        return;
    }
    tiempoRestante--;
    actualizarHUD();
}

void GameWidget::mostrarDerrota() {
    qDebug() << "=== MOSTRANDO DERROTA ===";
    if (estadoHUD) estadoHUD->setText("Estado: Derrota");

    QGraphicsSimpleTextItem* texto = new QGraphicsSimpleTextItem("¡Has perdido!");
    texto->setBrush(Qt::red);
    texto->setFont(QFont("Arial", 24));
    texto->setPos(250, 250);
    texto->setZValue(100);
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
    texto->setZValue(100);
    scene->addItem(texto);

    if (botonReiniciar) botonReiniciar->show();
    updateTimer.stop();
    nivelTimer.stop();

    emit nivelCompletado();
}

void GameWidget::reiniciarNivel() {
    qDebug() << "=== REINICIANDO NIVEL ===";
    inicializarNivel(nivelActual);
    updateTimer.start(16);
    if (nivelActual != 2) nivelTimer.start(1000);
}

void GameWidget::crearHUD() {
    if (!scene) return;

    if (tiempoHUD) { scene->removeItem(tiempoHUD); delete tiempoHUD; }
    if (explosionesHUD) { scene->removeItem(explosionesHUD); delete explosionesHUD; }
    if (estadoHUD) { scene->removeItem(estadoHUD); delete estadoHUD; }
    if (vidaBarra) { scene->removeItem(vidaBarra); delete vidaBarra; }

    tiempoHUD = new QGraphicsSimpleTextItem("Tiempo: 10"); // Ahora empieza en 10
    tiempoHUD->setBrush(Qt::yellow);
    tiempoHUD->setFont(QFont("Arial", 16));
    tiempoHUD->setPos(10, 10);
    scene->addItem(tiempoHUD);

    explosionesHUD = new QGraphicsSimpleTextItem("Enemigos: 0");
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
