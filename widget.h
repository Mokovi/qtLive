#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QHostAddress>
#include <QMessageBox>
#include "databasemanager.h"
#include "livetcpsocket.h"
#include <QVector>
#include "pack.h"
#include <QStringList>
#include <QDebug>
#include <QRegularExpression> //正则表达式，用来实现邮箱规范检测

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initNetwork();
    LiveTcpSocket* findSocket(QTcpSocket* tcpSocket);
    bool userLogIn(LiveTcpSocket* liveSocket, Pack pack);
    bool userSignIn(LiveTcpSocket* liveSocket, Pack pack);
    bool checkNameRule(QString& username) const;
    bool checkPwdRule(QString& password) const;
    bool checkEmailRule(QString& email) const;


private slots:
    void handleNewConnection();
    void handleDisconnection();
    void getData();

private:
    Ui::Widget *ui;
    QTcpServer *serverSocket;
    DatabaseManager& db = DatabaseManager::getInstance();
    QVector<LiveTcpSocket*> LiveSocketsVector;

};
#endif // WIDGET_H
