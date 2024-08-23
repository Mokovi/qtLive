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

}


