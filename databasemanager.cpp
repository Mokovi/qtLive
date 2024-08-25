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

    if (database.open()) {
        qDebug() << "数据库连接成功.";

        // 执行 SQL 语句，将所有 onlineState 设置为 'offline'
        QSqlQuery query(database);
        query.prepare("UPDATE accountinfo SET onlineState = 'offline'");

        if (query.exec()) {
            qDebug() << "所有用户状态已重置为 offline.";
        } else {
            qDebug() << "重置用户状态失败:" << query.lastError().text();
        }
    }
    else {
        qDebug() << "数据库连接失败.";
    }
}


SignStatus DatabaseManager::userSignIn(const QString &username, const QString &password, const QString &email)
{
    // 检查用户名是否已经存在
    QSqlQuery query(database);
    query.prepare("SELECT COUNT(*) FROM accountinfo WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec() || !query.next()) {
        return SIGN_DB_ERROR;
    }
    if (query.value(0).toInt() > 0) {
        return SIGN_NAME_EXIST_ERROR;
    }

    // 检查电子邮件是否已经存在
    query.prepare("SELECT COUNT(*) FROM accountinfo WHERE email = :email");
    query.bindValue(":email", email);
    if (!query.exec() || !query.next()) {
        return SIGN_DB_ERROR;
    }
    if (query.value(0).toInt() > 0) {
        return SIGN_EMAIL_EXIST_ERROR;
    }

    // 生成盐值
    QString salt = generateSalt();  // 假设有一个函数来生成随机盐值
    QString hashedPassword = hashPassword(password, salt);  // 假设有一个函数用盐值生成哈希密码

    // 插入新用户到数据库
    query.prepare("INSERT INTO accountinfo (username, email, password_hash, salt) "
                  "VALUES (:username, :email, :password_hash, :salt)");
    query.bindValue(":username", username);
    query.bindValue(":email", email);
    query.bindValue(":password_hash", hashedPassword);
    query.bindValue(":salt", salt);

    if (!query.exec()) {
        return SIGN_DB_ERROR;
    }

    return SIGN_SUCCESS;
}

LogStatus DatabaseManager::userLogIn(const QString& username, const QString& password)
{
    QSqlQuery query(database);

    // 1. 检查用户名是否存在
    query.prepare("SELECT password_hash, salt, onlineState FROM accountinfo WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        return LOG_DB_ERROR;  // 数据库操作错误
    }

    if (!query.next()) {
        return LOG_INPUT_NAME_ERROR;  // 用户名不存在
    }

    // 2. 检查是否重复登录
    QString onlineState = query.value("onlineState").toString();
    if (onlineState == "online") {
        return LOG_REAPEAT_LOG_ERROR;  // 重复登录
    }

    // 3. 获取数据库中的哈希密码和盐值
    QString storedHash = query.value("password_hash").toString();
    QString storedSalt = query.value("salt").toString();

    // 4. 验证输入的密码
    QString computedHash = hashPassword(password, storedSalt);
    if (computedHash != storedHash) {
        return LOG_INPUT_PWD_ERROR;  // 密码错误
    }

    // 5. 登录成功，更新用户状态为在线
    query.prepare("UPDATE accountinfo SET onlineState = 'online', last_login = CURRENT_TIMESTAMP WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        return LOG_DB_ERROR;  // 数据库操作错误
    }

    return LOG_SUCCESS;  // 登录成功
}




DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{

}

QString DatabaseManager::generateSalt(int length)
{
    QByteArray salt;
    for (int i = 0; i < length; ++i) {
        salt.append(QRandomGenerator::global()->bounded(256));  // 生成随机字节
    }
    return salt.toBase64();  // 返回Base64编码的字符串
}

QString DatabaseManager::hashPassword(const QString &password, const QString &salt)
{
    QByteArray saltedPassword = password.toUtf8() + salt.toUtf8();  // 组合密码与盐值
    QByteArray hash = QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256);  // 进行SHA-256哈希处理
    return hash.toHex();  // 返回16进制字符串
}
