#include <QtCore/QCoreApplication>
#include "server.h"
#include "db.h"

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);

    Server server;
    server.startServer();
    return app.exec();
}
