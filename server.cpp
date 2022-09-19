#include "server.h"
#include "db.h"

// constructor
Server::Server(){
    db = new Db("D:/db_chat_server1");
}

// destructor
Server::~Server() {}

// server is waiting for cleints to connect
void Server::startServer(){
    if (this->listen(QHostAddress::Any, 8080)){
        qDebug() << "Listening";
    } else {
        qDebug() << "NOT Listening";
    }
}

// if client tries to connect with server
void Server::incomingConnection(int socketDescriptor){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    qDebug() << socketDescriptor << "Client connected";
}

// when data were written to socket (recieved from client)
void Server::sockReady(){
    QByteArray data = socket->readAll();
    QString str = QByteArray(data);

    // attempt to register from client
    if (str.startsWith("New user info:")) {
        qDebug() << "SERVER: new user attempts to register";
        str.remove(0, 15);
        QStringList list = str.split("\n");
        qDebug() << "SERVER: List of user`s registration info: "<< list;
        QString name = list.value(0);
        QSqlQuery querySelect;
        querySelect.prepare("SELECT name from Users");
        if (!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }

        // creating list of users
        querySelect.first();
        QString users_list;
        while(querySelect.next()) {
            QString name_query = querySelect.value(0).toString();
            if (QString::compare(name_query, name, Qt::CaseSensitive)!=0) {
                users_list.append(querySelect.value(0).toString());
                users_list.append("\n");
            }
        }
        users_list.append(name);

        // if name exists in database, user won`t be created
        querySelect.first();
        while(querySelect.next()) {
            QString name_query = querySelect.value(0).toString();
            if (QString::compare(name_query, name, Qt::CaseSensitive)==0) {
                socket->write("falseRegister");
                qDebug() << "False: user with this name already exists";
                return;
            }
        }
        qDebug() << "True: user will be created";
        QByteArray buffer;
        buffer.append("trueRegister");
        buffer.append(users_list);
        socket->write(buffer);
        db->insertUser(list.value(0), list.value(1), list.value(2), list.value(3));
    }

    // attempt to log in from client
    else if (str.startsWith("Login info")) {
        qDebug() << "SERVER: user attempts to log in";
        str.remove(0, 12);
        QStringList list = str.split("\n");
        qDebug() << "SERVER: List of user`s login info: "<< list;
        QString login = list.value(0);
        QString password = list.value(1);
        QSqlQuery querySelect;
        querySelect.prepare("SELECT name, login, password from Users");
        if(!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }

        // creating list of users
        querySelect.first();
        QString users_list;
        QString last_name;
        while(querySelect.next()) {
            QString login_query = querySelect.value(1).toString();
            QString password_query = querySelect.value(2).toString();
            if (QString::compare(login_query, login, Qt::CaseSensitive)==0 &&
                QString::compare(password_query, password, Qt::CaseSensitive)==0) {
                last_name = querySelect.value(0).toString();
            } else {
                users_list.append(querySelect.value(0).toString());
                users_list.append("\n");
            }
        }
        users_list.append(last_name);

        // checking if login and password are valid
        querySelect.first();
        while(querySelect.next()) {
            QString login_query = querySelect.value(1).toString();            
            QString password_query = querySelect.value(2).toString();
            if (QString::compare(login_query, login, Qt::CaseSensitive)==0 &&
                QString::compare(password_query, password, Qt::CaseSensitive)==0) {
                QByteArray buffer;
                buffer.append("trueLogin");
                buffer.append(users_list);
                socket->write(buffer);
                qDebug() << "True: user is valid";
                return;
            }
        }
        qDebug() << "False: no such user in database";
        socket->write("falseLogin");
    }

    // if user pressed "Send" in ChatWindow
    else if (str.startsWith("newMessage")) {
        qDebug() << "SERVER: User attempts to send message";
        str.remove(0, 10);
        QStringList list = str.split("\n");
        db->insertMessage(list.value(0), list.value(1), list.value(2), list.value(3));
    }

    // if user clicked on list item in ChatWindow
    else if (str.startsWith("messagesHistory")) {
        qDebug() << "SERVER: User attempts to get all messages for clicked person";
        str.remove(0, 15);
        QStringList list = str.split("\n");
        qDebug() << list;
        QString result = "allMessages";
        QSqlQuery querySelect;
        QByteArray buffer;
        QString query = "SELECT date_send, sender, message, reciever FROM Messages";
        querySelect.prepare(query);
        if(!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }
        buffer.append(result);
        while (querySelect.next()) {
            QString sender = querySelect.value(1).toString();
            QString reciever = querySelect.value(3).toString();
            if ((QString::compare(list.value(0), sender, Qt::CaseSensitive)==0 &&
                 QString::compare(list.value(1), reciever, Qt::CaseSensitive)==0) ||
                (QString::compare(list.value(1), sender, Qt::CaseSensitive)==0 &&
                 QString::compare(list.value(0), reciever, Qt::CaseSensitive)==0)) {
                buffer.append(querySelect.value(0).toString());
                buffer.append("| ");
                buffer.append(querySelect.value(1).toString());
                buffer.append("| ");
                buffer.append(querySelect.value(2).toString());
                buffer.append("\n");
            }
        }
        if (buffer.size() == 11) {
            buffer.append("Write first message to this user");
        }
        socket->write(buffer);
    }

    // 1st query from ChatBot
    else if (str.startsWith("chatBot0")) {
        QByteArray buffer;
        buffer.append("chatBot0Result");
        QSqlQuery querySelect;
        QString query = "SELECT name, role FROM Users";
        querySelect.prepare(query);
        if (!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }
        buffer.append("All users:\n");
        while (querySelect.next()) {
            buffer.append(querySelect.value(0).toString());
            buffer.append(" | ");
            buffer.append(querySelect.value(1).toString());
            buffer.append("\n");
        }
        socket->write(buffer);
    }

    // 2nd query from ChatBot
    else if (str.startsWith("chatBot1")) {
        QByteArray buffer;
        buffer.append("chatBot1Result");
        QSqlQuery querySelect;
        QString query = "SELECT name, role FROM Users";
        querySelect.prepare(query);
        if (!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }
        buffer.append("All programmers:\n");
        while (querySelect.next()) {
            if ((QString::compare(querySelect.value(1).toString(),
                                  "programmer", Qt::CaseInsensitive)==0 )) {
                buffer.append(querySelect.value(0).toString());
                buffer.append(" | ");
                buffer.append(querySelect.value(1).toString());
                buffer.append("\n");
            }
        }
        socket->write(buffer);
    }

    // 3rd query from ChatBot
    else if (str.startsWith("chatBot2")) {
        QByteArray buffer;
        buffer.append("chatBot2Result");
        QSqlQuery querySelect;
        QString query = "SELECT name, role FROM Users";
        querySelect.prepare(query);
        if (!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }
        buffer.append("All trainees:\n");
        while (querySelect.next()) {
            if ((QString::compare(querySelect.value(1).toString(),
                                  "trainee", Qt::CaseInsensitive)==0 )) {
                buffer.append(querySelect.value(0).toString());
                buffer.append(" | ");
                buffer.append(querySelect.value(1).toString());
                buffer.append("\n");
            }
        }
        socket->write(buffer);
    }

    // 4th query from ChatBot
    else if (str.startsWith("chatBot3")) {
        QByteArray buffer;
        buffer.append("chatBot3Result");
        QSqlQuery querySelect;
        QString query = "SELECT name, role FROM Users";
        querySelect.prepare(query);
        if (!querySelect.exec()) {
            qDebug() << "SERVER: Can`t execute select query";
        }
        buffer.append("Other users:\n");
        while (querySelect.next()) {
            if ((QString::compare(querySelect.value(1).toString(),
                                  "programmer", Qt::CaseInsensitive)!=0 &&
                 QString::compare(querySelect.value(1).toString(),
                                  "trainee", Qt::CaseInsensitive)!=0 )) {
                buffer.append(querySelect.value(0).toString());
                buffer.append(" | ");
                buffer.append(querySelect.value(1).toString());
                buffer.append("\n");
            }
        }
        socket->write(buffer);
    }
}

// socket disconnection
void Server::sockDisc(){
    qDebug() << "Disconnect";
    socket->deleteLater();
}
