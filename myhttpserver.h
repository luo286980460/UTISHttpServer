#ifndef MYHTTPSERVER_H
#define MYHTTPSERVER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include "include/libhv/HttpServer.h"

using namespace hv;

class Controller;

class MyHttpServer : public QObject
{
    Q_OBJECT

public:
    MyHttpServer(int port, QObject *parent = nullptr);
    virtual ~MyHttpServer();

    void updateControllList(QList<Controller*>* controllList);      // 更新警示灯控制器列表
    void setCfgJson(QJsonObject& cfgJson);
    void stop();

private:
    void createHttpserver(int port);                // 开启httpserver
    bool ipAddrIsOK(const QString & ip);            // 判断字符串是否为合法 ip 地址
    QString qstr2Hex(QString instr);                // 汉字 转 16进制
    bool missingParameter(QJsonObject& json,        // 是否缺少参数
                          QJsonObject& backJson);
    bool missingParameterBroadcast(QJsonObject& json,       // parseLightBroadcast 是否缺少必要参数
                                   QJsonObject& backJson);
    QJsonObject parseLightBroadcast(QJsonObject& json);     // 解析 雾灯 广播控灯（协议3.0）
    bool missingParameterBroadcastNot(QJsonObject& json,    // parseLightBroadcast 是否缺少必要参数
                                      QJsonObject& backJson);

    QJsonObject parseLightBroadcastNot(QJsonObject& json);  // 解析 雾灯 非广播控灯（协议3.0）
    bool ipPortIsOK(QString ipPort);
    bool controllerIsUseful(Controller* controller,         // 控制器是否存在，或者是否处在查询状态
                            QString TermIp,
                            QJsonObject &backJson);
    Controller* getControllerFromIpPort(QString ip,         // 从ip和Port获取控制器对象
                                        int Port);
    QJsonObject parseUpdateLightState(QJsonObject& json);  // 解析 雾灯 轨迹请求
    QJsonObject parseLightPathTracking(QJsonObject& json);  // 解析 雾灯 轨迹请求

    // 旧版协议
    QJsonObject parseLightJson(QJsonObject& json);          // 解析 雾灯 json数据 2.0 版本

    void add_file_handler(HttpServer& server, const QString& basepath, const QString& path);
    void add_directory_handlers(HttpServer& server, const QString& basepath, const QString& path = "");
    QByteArray crypt_Aes128_ECB_PKCS7_HEX(QByteArray plaintext, QByteArray key);
    QByteArray decrypt_Aes128_ECB_PKCS7_HEX(QByteArray plaintext, QByteArray key);

    // // 防断电同步命令
    // void reSendCmd();   // 防止设备断电以后没有内容，定时发送最后的状态

    bool headerIsOk(HttpRequest* req, QJsonObject& backJson);
    int respReturnJson(HttpResponse* resp, QJsonObject json);

signals:
    void signalWrite2Kafka(QString topic, QString strJson, QString strKey);
    void signalSetCfgJson(QByteArray cfgJson);
    void signalRestartApplication();

public slots:

public:
    hv::HttpServer* m_httpServer;
    HttpService* m_router;
    QList<Controller*>* m_controllList = nullptr;   // 警示灯控制器列表
    QJsonObject m_cfgJson;

private:
    QByteArray m_aesKey = "Utis00000000LsCb";
};


#endif // MYHTTPSERVER_H
