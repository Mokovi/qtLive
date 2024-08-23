#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>

class DatabaseManager
{
public:
    static DatabaseManager &getInstance();
    void initDatabase();

private:
    DatabaseManager();
    ~DatabaseManager();


private:
    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
