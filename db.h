#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>

class Db {

protected:
    QSqlDatabase db;

public:
    Db(const QString &);
    ~Db();

    // prototypes for "Users" table
    bool isOpen() const;
    bool createUsersTable(void);
    bool insertUser(const QString &name,
                    const QString &login, const QString &password, const QString &role);
    bool removeUser(const int &userId);
    bool updateUser(const int &userId, const QString &name,
                    const QString &login, const QString &password, const QString &role);
    bool showAllUsers();

    // prototypes for "Messages" table
    bool createMessagesTable(void);
    bool insertMessage(const QString &date_send,
                       const QString &sender, const QString &message,
                       const QString &reciever);
    bool removeMessage(const int &messageId);
    bool updateMessage(const int &messageId,
                       const QString &date_send, const QString &sender,
                       const QString &message, const QString &reciever);
    bool showAllMessages(void);

};

#endif // DB_H
