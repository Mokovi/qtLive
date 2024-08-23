#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
/*
服务器：
    1.采用TCP通信，继承TCP创建一个自定义套接字，套接字中包含客户信息
    2.心跳检测，服务器若发送信号给客户端后，若10s内未收到回复，则视为客户端已经断开，将客户端状态置0
    3.创建一个pack类，用来进行通信过程中的数据打包
        a.包的内容包括：数据包整体大小packSize,操作行为:operationType,返回状态returnStatus,数据缓存区:dataBuff[1024]
        b.oeprationType包括：登录，注册
        c.returnStatus是个uchar类型,存储对应operationType的返回状态值
        d.在数据读取时需要注意数据的分包以及粘包
    4.数据库user表内容：
        id (INT, PRIMARY KEY, AUTO_INCREMENT): 用户的唯一标识符，通常是自增的整数。
        username (VARCHAR): 用户名，用于用户登录。应确保唯一性（UNIQUE约束）。
        email (VARCHAR): 用户的电子邮件地址，用于找回密码等。应确保唯一性（UNIQUE约束）。
        password_hash (VARCHAR): 存储用户密码的哈希值，而非明文密码。建议使用安全的哈希算法，如 bcrypt、Argon2、PBKDF2 等。
        salt (VARCHAR): 如果使用盐值（Salt），可以存储在这个字段中。每个用户的盐值应是唯一的。
        created_at (TIMESTAMP): 账号创建时间。
        updated_at (TIMESTAMP): 账号信息最近更新的时间。
        last_login (TIMESTAMP): 最近一次登录的时间。
        status (TINYINT): 账号状态（例如，0=未激活，1=激活，2=封禁等）。
    5.客户端中创建使用listview创建一个用户列表，点击相应用户可以在textbrowser中查看其状态及行为。

*/
