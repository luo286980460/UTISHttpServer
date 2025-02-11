#ifndef MYHTTPSERVER_H
#define MYHTTPSERVER_H

#include <QObject>
#include <QDebug>
//#include <QHttpServer>
#include "include/libhv/HttpServer.h"

class MyHttpServer : public QObject
{
    Q_OBJECT

public:
    MyHttpServer(QObject *parent = nullptr);
    virtual ~MyHttpServer();

private:
    void createHttpserver(int port);                // 开启httpserver
    bool ipAddrIsOK(const QString & ip);            // 判断字符串是否为合法 ip 地址
    //QString qstr2Hex(QString instr);                // 汉字 转 16进制
    // QJsonObject parseLightJson(QJsonObject& json);  //解析 雾灯 json数据 2.0 版本
    bool missingParameter(QJsonObject& json,        // 是否缺少参数
                          QJsonObject& backJson);

signals:

public slots:

public:
    hv::HttpServer* m_httpServer;
    HttpService* m_router;
};


#endif // MYHTTPSERVER_H
