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

bool Widget::userLogIn(LiveTcpSocket* liveSocket, Pack pack)
{
    qDebug() << "登录";
}

bool Widget::userSignIn(LiveTcpSocket* liveSocket, Pack pack)
{
    QStringList list = pack.getData();
    QString& username = list[0];
    QString& password = list[1];
    QString& email = list[2];
    //检测命名规则
    if(!checkNameRule(username)){
        pack.setSignStatus(SIGN_NAME_RULE_ERROR);
        return false;
    }
    if(!checkPwdRule(password)){
        pack.setSignStatus(SIGN_PWD_RULE_ERROR);
        return false;
    }
    if(!checkEmailRule(email)){
        pack.setSignStatus(SIGN_EMAIL_RULE_ERROR);
        return false;
    }
    //数据库检查

}

bool Widget::checkNameRule(QString &username) const
{
    if(username.length() < 3 || username.length() > 12)
    {
        return false;
    }
    else {
        return true;
    }
}

bool Widget::checkPwdRule(QString &password) const
{
    if(password.length() < 3 || password.length() > 12)
    {
        return false;
    }
    else {
        return true;
    }
}

bool Widget::checkEmailRule(QString &email) const
{
    QRegularExpression emailRegex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
    QRegularExpressionMatch match = emailRegex.match(email);

    return match.hasMatch();
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

        switch ((OperationType)pack.getOperationType())
        {
        case LOGIN:
            userLogIn(liveSocket, pack);
            break;

        case SIGNIN:
            userSignIn(liveSocket, pack);
            break;
        }
    }
}



