#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class MenuWidget : public QWidget {
    Q_OBJECT

public:
    explicit MenuWidget(QWidget* parent = nullptr);

signals:
    void nivelSeleccionado(int numeroNivel);
    void salirJuego();

private slots:
    void onBtnNivel1Clicked();
    void onBtnNivel2Clicked();
    void onBtnNivel3Clicked();
    void onBtnSalirClicked();

private:
    QVBoxLayout* layout;
    QLabel* titulo;
    QPushButton* btnNivel1;
    QPushButton* btnNivel2;
    QPushButton* btnNivel3;
    QPushButton* btnSalir;
};

#endif
