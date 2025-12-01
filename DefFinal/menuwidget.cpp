#include "menuwidget.h"

MenuWidget::MenuWidget(QWidget* parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    titulo = new QLabel("BATALLAS HISTÓRICAS", this);
    QFont font("Arial", 28, QFont::Bold);
    titulo->setFont(font);
    titulo->setStyleSheet("color: white;");
    titulo->setAlignment(Qt::AlignCenter);

    QString btnStyle = "QPushButton { background-color: #4CAF50; color: white; padding: 15px; "
                       "border-radius: 8px; font-size: 16px; min-width: 300px; }"
                       "QPushButton:hover { background-color: #45a049; }";

    btnNivel1 = new QPushButton("Nivel 1: Batalla de Kursk", this);
    btnNivel1->setStyleSheet(btnStyle);

    btnNivel2 = new QPushButton("Nivel 2: Batalla de Pearl Harbor", this);
    btnNivel2->setStyleSheet(btnStyle);

    btnNivel3 = new QPushButton("Nivel 3: Batalla de Londres", this);
    btnNivel3->setStyleSheet(btnStyle);

    btnSalir = new QPushButton("Salir", this);
    btnSalir->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 15px; "
                            "border-radius: 8px; font-size: 16px; min-width: 300px; }"
                            "QPushButton:hover { background-color: #da190b; }");

    layout->addWidget(titulo);
    layout->addSpacing(30);
    layout->addWidget(btnNivel1);
    layout->addWidget(btnNivel2);
    layout->addWidget(btnNivel3);
    layout->addSpacing(20);
    layout->addWidget(btnSalir);

    connect(btnNivel1, &QPushButton::clicked, this, &MenuWidget::onBtnNivel1Clicked);
    connect(btnNivel2, &QPushButton::clicked, this, &MenuWidget::onBtnNivel2Clicked);
    connect(btnNivel3, &QPushButton::clicked, this, &MenuWidget::onBtnNivel3Clicked);
    connect(btnSalir, &QPushButton::clicked, this, &MenuWidget::onBtnSalirClicked);

    setStyleSheet("background-color: #2c3e50;");
}

void MenuWidget::onBtnNivel1Clicked() { emit nivelSeleccionado(1); }
void MenuWidget::onBtnNivel2Clicked() { emit nivelSeleccionado(2); }
void MenuWidget::onBtnNivel3Clicked() { emit nivelSeleccionado(3); }
void MenuWidget::onBtnSalirClicked() { emit salirJuego(); }
