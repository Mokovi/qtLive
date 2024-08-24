#ifndef LIVETCPSOCKET_H
#define LIVETCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class LiveTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit LiveTcpSocket(QTcpSocket* tcpSocket, QObject *parent = nullptr);

    QTcpSocket * getTcpSocket();
    qint64 read(char* data, qint64 maxlen);
    qint64 write(const char* data, qint64 len);
    qint64 bytesAvailable();
    void close();

    void set_login_status(bool logInFlag);
    void set_username(QString username);

    const QString& get_username();

private:
    QTcpSocket * tcpSocket;
    QString username;
    bool logInFlag;

};

#endif // LIVETCPSOCKET_H
