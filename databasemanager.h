#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include "pack.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>
#include <QString>
#include <QSqlError>

class DatabaseManager
{
public:
    static DatabaseManager &getInstance();
    void initDatabase();
    SignStatus userSignIn(const QString& username, const QString& password, const QString& email);
    LogStatus userLogIn(const QString& username, const QString& password);

private:
    DatabaseManager();
    ~DatabaseManager();
    QString generateSalt(int length = 16);
    QString hashPassword(const QString &password, const QString &salt);


private:
    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
