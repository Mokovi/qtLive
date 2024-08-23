#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QHostAddress>
#include <QMessageBox>
#include "databasemanager.h"

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

private:
    Ui::Widget *ui;
    QTcpServer *serverSocket;
    DatabaseManager& db = DatabaseManager::getInstance();

};
#endif // WIDGET_H
