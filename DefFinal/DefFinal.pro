QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

HEADERS += \
    avionenemigo.h \
    barco.h \
    canon.h \
    controlspawn.h \
    enemigo.h \
    entidadfija.h \
    entidadjuego.h \
    entidadmovil.h \
    gamewidget.h \
    mainwindow.h \
    menuwidget.h \
    motoraprendizaje.h \
    nivel.h \
    nivel1.h \
    nivel2.h \
    nivellondres.h \
    obstaculo.h \
    proyectil.h \
    proyectilBalistico.h \
    soldado.h \
    spawnevent.h \
    tanquejugador.h

SOURCES += \
    avionenemigo.cpp \
    barco.cpp \
    canon.cpp \
    controlspawn.cpp \
    enemigo.cpp \
    entidadfija.cpp \
    entidadjuego.cpp \
    entidadmovil.cpp \
    gamewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    menuwidget.cpp \
    motoraprendizaje.cpp \
    nivel.cpp \
    nivel1.cpp \
    nivel2.cpp \
    nivellondres.cpp \
    obstaculo.cpp \
    proyectil.cpp \
    proyectilBalistico.cpp \
    soldado.cpp \
    tanquejugador.cpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


