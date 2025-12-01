QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Nivel.cpp \
    avionenemigo.cpp \
    entidadjuego.cpp \
    entidadmovil.cpp \
    gamewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    motoraprendizaje.cpp \
    nivellondres.cpp \
    proyectil.cpp \
    soldado.cpp

HEADERS += \
    Nivel.h \
    avionenemigo.h \
    entidadjuego.h \
    entidadmovil.h \
    gamewidget.h \
    mainwindow.h \
    motoraprendizaje.h \
    nivellondres.h \
    proyectil.h \
    soldado.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    audio


