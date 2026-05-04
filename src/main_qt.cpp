#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Nase UFPE");
    app.setApplicationVersion("1.0");

    MainWindow w;
    w.show();

    return app.exec();
}
