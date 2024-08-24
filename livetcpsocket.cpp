#include "livetcpsocket.h"



LiveTcpSocket::LiveTcpSocket(QTcpSocket *tcpSocket, QObject *parent)
    :QObject(parent)
    ,tcpSocket(tcpSocket)
    ,logInFlag(false)
{

}

QTcpSocket *LiveTcpSocket::getTcpSocket()
{
    return tcpSocket;
}

qint64 LiveTcpSocket::read(char *data, qint64 maxlen)
{
    return tcpSocket->read(data, maxlen);
}

qint64 LiveTcpSocket::write(const char *data, qint64 len)
{
    return tcpSocket->write(data, len);
}

qint64 LiveTcpSocket::bytesAvailable()
{
    return tcpSocket->bytesAvailable();
}

void LiveTcpSocket::close()
{
    tcpSocket->close();
}

void LiveTcpSocket::set_login_status(bool logInFlag)
{
    this->logInFlag = logInFlag;
}

void LiveTcpSocket::set_username(QString username)
{
    this->username = username;
}

const QString &LiveTcpSocket::get_username()
{
    return username;
}





