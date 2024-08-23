#include "databasemanager.h"

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

void DatabaseManager::initDatabase()
{
    database = QSqlDatabase::addDatabase("QODBC");
    database.setHostName("127.0.0.1");
    database.setPort(3306);
    database.setUserName("root");
    database.setPassword("MySQL4563140");
    database.setDatabaseName("pratice");

    if(database.open()){
        qDebug() << "数据库连接成功.";
    }
    else {
        qDebug() << "数据库连接失败.";
    }
}

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{

}