#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QSqlQuery>
#include <db.h>

class Server: public QTcpServer {
    Q_OBJECT

public:
    Server();
    ~Server();

    QTcpSocket* socket;
    //QSqlDatabase* db;
    Db* db;

public slots:
    void startServer();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // SERVER_H
