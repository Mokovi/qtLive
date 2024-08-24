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
