#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , serverSocket(new QTcpServer(this))
{
    ui->setupUi(this);
    initNetwork();
    db.initDatabase();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initNetwork()
{
    if(!serverSocket->listen(QHostAddress::AnyIPv4, 13145))
    {
        QMessageBox::warning(this, "警告", "开启服务器失败。");
    }
    else
    {
        QMessageBox::information(this, "通知", "开启服务器成功。");
    }
    connect(serverSocket, &QTcpServer::newConnection, this, &Widget::handleNewConnection);
}

LiveTcpSocket* Widget::findSocket(QTcpSocket *tcpSocket)
{
    for(auto liveSocket : LiveSocketsVector){
        if(liveSocket->getTcpSocket() == tcpSocket){
            return liveSocket;
        }
    }
    return nullptr;
}

void Widget::handleNewConnection()
{
    QTcpSocket *newClientSocket = serverSocket->nextPendingConnection();
    LiveTcpSocket *newLiveSocket = new LiveTcpSocket(newClientSocket);
    LiveSocketsVector.push_back(newLiveSocket);
    connect(newClientSocket, &QTcpSocket::readyRead, this, &Widget::getData);
    connect(newClientSocket, &QTcpSocket::disconnected, this, &Widget::handleDisconnection);

}

void Widget::handleDisconnection()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    LiveTcpSocket* liveSocket = findSocket(clientSocket);
    if(liveSocket)
    {
        LiveSocketsVector.removeOne(liveSocket);
        liveSocket->deleteLater();//来自于qobject
    }
}

void Widget::getData()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    LiveTcpSocket* liveSocket = findSocket(clientSocket);
    if (!liveSocket) return;

    while (liveSocket->bytesAvailable() > 4) // 确保有足够的字节解析包头
    {
        Pack pack;
        unsigned int size = 0;

        int rst = liveSocket->read((char*)&size, 4); // 读取包头（4字节）
        qDebug() << rst << "\t" << size;
        if(rst == 0 || size <= 4) break; // 如果没有数据或包大小无效，退出循环

        // 确保数据已完全接收
        if (liveSocket->bytesAvailable() < size - 4) {
            return; // 数据不足，等待更多数据到来
        }

        liveSocket->read(pack.data() + 4, size - 4); // 读取包体数据

        QStringList strlist = pack.getData();

        // 确保有足够的字符串数据
        for(auto str : strlist){
            qDebug() << str;
        }


        /*
        switch ((OperationType)pack.getOperationType())
        {
        case LOGIN:
            // user_register(liveSocket, pack);
            break;

        case SIGNIN:
            // user_login(liveSocket, pack);
            break;
        }
        */
    }
}



