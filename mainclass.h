#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QJsonObject>
#include <QTcpSocket>

class MyHttpServer;
class producer;
class Controller;   //

class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QObject *parent = nullptr);

    void init();
    bool initCfgJson();     // 初始化配置文件json
    bool initHttpserver();  // 初始化HttpServer
    bool initProducer();    // 初始化kafka
    bool initControlls();   // 初始化雾灯控制器
    // bool initTcpClient();   // 初始化tcp客户端

private:

signals:

public slots:
    void slotWrite2Kafka(QString topic,
                         QString strJson,
                         QString strKey);
    void slotSetCfgJson(QByteArray cfgJson);

private:
    QJsonObject m_cfgJson;
    MyHttpServer* m_myHttpServer;           // http
    QList<producer*> m_producerList;        // kafka创造者列表
    QList<Controller*> m_controllList;      // 警示灯控制器列表
    QTcpSocket* m_tcpClient;                // tcp客户端，用来连接web后端服务
    QString m_webServerIp;                  // web服务ip
    int m_webServerPort;                    // web服务ip
};

#endif // MAINCLASS_H
