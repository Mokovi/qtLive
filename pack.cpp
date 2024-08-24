#include "pack.h"
#include <cstring>  // 包含memset, memcpy等函数

Pack::Pack()
    : packSize(6)  // 前6个字节为包头
    , operationType(INVALID_TYPE)
    , status(INVALID_LOG_STATUS)
{
    memset(dataBuff, 0, sizeof(dataBuff));
}

Pack::~Pack()
{
}

void Pack::clear()
{
    packSize = 6;
    operationType = INVALID_TYPE;
    status = INVALID_LOG_STATUS;
    memset(dataBuff, 0, sizeof(dataBuff));
}

QStringList Pack::getData()
{
    unsigned int read_count = 0;
    unsigned short size = 0;
    QStringList strlist;

    while(true)
    {
        if (read_count + 2 > sizeof(dataBuff)) break;  // 检查缓冲区是否越界
        memcpy(&size, dataBuff + read_count, 2);
        read_count += 2;

        if (read_count + size > sizeof(dataBuff)) break;  // 检查缓冲区是否越界
        QByteArray temp(dataBuff + read_count, size);
        read_count += size;

        QString strdata = QString::fromLocal8Bit(temp);
        if(!strdata.isEmpty())
        {
            strlist.append(strdata);
        }
        if(0 == dataBuff[read_count])  // 如果没有更多数据，退出循环
        {
            break;
        }
    }

    return strlist;
}

void Pack::append(const QString& val)
{
    QByteArray byteArray = val.toLocal8Bit();
    unsigned short size = byteArray.size();

    if (packSize - 6 + 2 + size > sizeof(dataBuff))  // 检查缓冲区是否有足够空间
    {
        // 错误处理：缓冲区空间不足
        return;
    }

    memcpy(dataBuff + (packSize-6), &size, 2); // 复制size数据
    packSize += 2;

    memcpy(dataBuff + (packSize-6), byteArray.data(), size); // 复制字符串数据
    packSize += size;
}



