#include "mainwindow.h"

#include <QApplication>

#include <csignal>

void crashHandler(int signal) {
    qFatal("\n========== CRASH! Señal %d ==========\n", signal);
    abort();
}

int main(int argc, char *argv[]) {
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
