#include "db.h"

// connection to database (constructor)
Db::Db(const QString &path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (!db.open())
        qDebug() << "Error: connection with database failed";
    else
        qDebug() << "Database: connection is OK";
}

// destructor
Db::~Db() {
    if (db.isOpen())
        db.close();
}

// checks if connection with database was opened
bool Db::isOpen() const {
    return db.isOpen();
}

// creating table "Users"
bool Db::createUsersTable() {
    bool success = true;
    QSqlQuery query;
    query.prepare("CREATE TABLE Users ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name TEXT, "
                  "login TEXT, "
                  "password TEXT, "
                  "role TEXT)");
    if (!query.exec()) {
        qDebug() << "This table has been already created, "
                "or issues with creating table";
        success = false;
    }
    return success;
}

// inserting row (user) into table "Users"
bool Db::insertUser(const QString &name, const QString &login,
                    const QString &password, const QString &role) {
    bool success = false;
    if (!name.isEmpty()){
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO Users (name, login, password, role)"
                         "VALUES (:name, :login, :password, :role) ");
        queryAdd.bindValue(":name", name);
        queryAdd.bindValue(":login", login);
        queryAdd.bindValue(":password", password);
        queryAdd.bindValue(":role", role);

        if (queryAdd.exec()) {
            success = true;
            qDebug() << "Record was added!";
        }
        else {
            qDebug() << "record wasn`t added:" << queryAdd.lastError();
        }
    } else
        qDebug() << "Data is required to insert.";

    return success;
}

// removing a row (user) from table "Users"
bool Db::removeUser(const int &userId) {
    bool success = false;
    if (userId > -1) {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM Users WHERE id="
                            + QString::number(userId));
        success = queryDelete.exec();

        if (!success)
            qDebug() << "Record hasn`t been deleted due to error: "
                    << queryDelete.lastError();
        else
            qDebug() << "Record was deleted successfully!";
    } else
        qDebug() << "Record has not been deleted: incorrect Id";
    return success;
}

// editing a row (user) in table "Users"
bool Db::updateUser(const int &userId, const QString &name,
                    const QString &login, const QString &password,
                    const QString &role) {
    bool success  = false;
    if (userId > -1) {
        QString query = "UPDATE Users"
                        " SET "
                        "name= :name, "
                        "login= :login, "
                        "password= :password, "
                        "role= :role "
                        "WHERE id=" + QString::number(userId);

        QSqlQuery queryEdit;
        queryEdit.prepare(query);
        queryEdit.bindValue(":name", name);
        queryEdit.bindValue("login", login);
        queryEdit.bindValue(":password", password);
        queryEdit.bindValue(":role", role);

        if (queryEdit.exec()) {
            success = true;
            qDebug() << "User`s info updated successfully!";
        } else
            qDebug() << "User wasn`t updated due to error: "
                    << queryEdit.lastError();
    } else
        qDebug() << "User Id is required to update...";
    return success;
}

// example of `SELECT` query
bool Db::showAllUsers() {
    bool success = true;
    QSqlQuery querySelect;
    querySelect.prepare("SELECT * from Users");
    if(!querySelect.exec()) {
        qDebug() << "Can`t execute this query";
        success = false;
    }
    querySelect.first();
    while(querySelect.next()) {
        QString id = querySelect.value(0).toString();
        QString name = querySelect.value(1).toString();
        QString login = querySelect.value(2).toString();
        QString password = querySelect.value(3).toString();
        QString role = querySelect.value(4).toString();
        qDebug()<<"Id"<<id;
        qDebug()<<"Name"<<name;
        qDebug()<<"login"<<login;
        qDebug()<<"password"<<password;
        qDebug()<<"Role"<< role;
        qDebug()<<"\n";
    }
    return success;
}

// creating table "Messages"
bool Db::createMessagesTable() {
    bool success = true;
    QSqlQuery query;
    query.prepare("CREATE TABLE Messages ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "date_send TEXT, "
                  "sender TEXT, "
                  "message TEXT, "
                  "reciever TEXT)");
    if (!query.exec()) {
        qDebug() << "This table has been already created, "
                "or issues with creating table";
        success = false;
    }
    return success;
}

// inserting row (message) into table "Messages"
bool Db::insertMessage(const QString &date_send, const QString &sender,
                       const QString &message, const QString &reciever) {
    bool success = false;
    if (!message.isEmpty()){
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO Messages"
                         " (date_send, sender, message, reciever)"
                         " VALUES (:date_send, :sender, :message, :reciever) ");
        queryAdd.bindValue("date_send", date_send);
        queryAdd.bindValue(":sender", sender);
        queryAdd.bindValue(":message", message);
        queryAdd.bindValue(":reciever", reciever);

        if (queryAdd.exec()) {
            success = true;
            qDebug() << "Record was added!";
        }
        else {
            qDebug() << "record wasn`t added:" << queryAdd.lastError();
        }
    } else
        qDebug() << "Data is required to insert.";
    return success;
}

// removing a row (message) from table "Messages"
bool Db::removeMessage(const int &messageId) {
    bool success = false;
    if (messageId > -1) {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM Messages"
                            " WHERE id=" + QString::number(messageId));
        success = queryDelete.exec();

        if (!success)
            qDebug() << "Record hasn`t been deleted due to error: "
                    << queryDelete.lastError();
        else
            qDebug() << "Record was deleted successfully!";
    } else
        qDebug() << "Record has not been deleted: incorrect Id";
    return success;
}

// editing a row (message) in table "Messages"
bool Db::updateMessage(const int &messageId, const QString &date_send,
                       const QString &sender, const QString &message,
                       const QString &reciever) {
    bool success  = false;
    if (messageId > -1) {
        QString query = "UPDATE Messages"
                        " SET "
                        "date_send= :date_send, "
                        "sender= :sender, "
                        "message= :message, "
                        "reciever= :reciever "
                        "WHERE id=" + QString::number(messageId);

        QSqlQuery queryEdit;
        queryEdit.prepare(query);
        queryEdit.bindValue(":date_send", date_send);
        queryEdit.bindValue(":sender", sender);
        queryEdit.bindValue(":message", message);
        queryEdit.bindValue(":reciever", reciever);

        if (queryEdit.exec()) {
            success = true;
            qDebug() << "User`s info updated successfully!";
        } else
            qDebug() << "User wasn`t updated due to error: "
                    << queryEdit.lastError();
    } else
        qDebug() << "User Id is required to update...";
    return success;
}

// example of `SELECT` query
bool Db::showAllMessages() {
    bool success = true;
    QSqlQuery querySelect;
    querySelect.prepare("SELECT * from Messages");
    if(!querySelect.exec()) {
        qDebug() << "Can`t execute this query";
        success = false;
    }
    qDebug() << "MESSAGES: ";
    querySelect.first();
    while(querySelect.next()) {
        QString id = querySelect.value(0).toString();
        QString date_send = querySelect.value(1).toString();
        QString sender = querySelect.value(2).toString();
        QString message = querySelect.value(3).toString();
        QString reciever = querySelect.value(4).toString();
        qDebug()<<"Id"<<id;
        qDebug()<<"date_send"<<date_send;
        qDebug()<<"sender"<<sender;
        qDebug()<<"message"<<message;
        qDebug()<<"reciever"<<reciever;
        qDebug()<<"\n";
    }
    return success;
}
