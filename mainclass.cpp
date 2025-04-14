#include "mainclass.h"
#include "myhttpserver.h"
#include "producer.h"
#include "controller.h"

#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>
#include <QJsonDocument>

#define CFG_JSON        "/cfg.json"


MainClass::MainClass(QObject *parent)
    : QObject{parent}
{
    init();
}

void MainClass::init()
{
    if(!initCfgJson()) return;  // 先读取配置， 往下的函数会使用配置信息

    if(!initProducer()) return;
    if(!initControlls()) return;
    if(!initHttpserver()) return;
    // if(!initTcpClient()) return;

}

bool MainClass::initCfgJson()
{
    // CFG_JSON配置文件是否存在
    QString iniPath = QCoreApplication::applicationDirPath() + CFG_JSON;

    if(!QFileInfo::exists(iniPath)){
        qCritical() << ("****** cfg.json 配置文件丢失 ******");
        return false;
    }

    // 打开配置文件
    QFile file(iniPath);
    if(!file.open(QIODevice::ReadWrite)){
        qCritical() << ("****** " + iniPath + " 配置文件打开失败 ******");
        return false;
    }

    // 读取配置文件
    QJsonParseError error;
    m_cfgJson = QJsonDocument::fromJson(file.readAll(), &error)
                    .object();
                    // .value("utisDeviceServer")
                    // .toObject();

    if(error.error != QJsonParseError::NoError){
        qCritical() << "error cfgJson: " + error.errorString();
        return false;
    }

    // 子参数 light/kafka 是否为json
    // if(!m_cfgJson.value("light").isObject() || !m_cfgJson.value("kafka").isObject()){
    //     qCritical() << "light/kafka not a json";
    //     return false;
    // }

    return true;
}

bool MainClass::initHttpserver()
{
    // int port = m_cfgJson.value("port").toInt();
    int port = m_cfgJson.value("serverPort").toString().toInt();

    if(port < 1) {
        qCritical() << " error UtisDeviceServer Port = " + QString::number(port);
        return false;
    }

    m_myHttpServer = new MyHttpServer(port, this);
    m_myHttpServer->setCfgJson(m_cfgJson);
    connect(m_myHttpServer, &MyHttpServer::signalWrite2Kafka, this, &MainClass::slotWrite2Kafka);
    connect(m_myHttpServer, &MyHttpServer::signalSetCfgJson, this, &MainClass::slotSetCfgJson);


    m_myHttpServer->updateControllList(&m_controllList);
    return true;
}

bool MainClass::initProducer()
{
    producer* producerTmp = nullptr;
    QString kafkaIpPort = m_cfgJson.value("kafkaIp").toString() + ":" + m_cfgJson.value("kafkaPort").toString();
    QStringList topicList = m_cfgJson.value("kafkaTopics").toString().split(",");

    for(int i=0; i<topicList.size(); i++){
        QString strTopic = topicList.at(i); //kafka主题
        producerTmp = new producer(this);
        //connect(this, &MainClass::write2Kafka, producerTmp, &producer::slotProduceMessvoidJson);
        producerTmp->setTopic(strTopic);
        producerTmp->setkafkaIp(kafkaIpPort);
        producerTmp->start();
        producerTmp->init();
        emit producerTmp->signalInitWork();
        m_producerList.append(producerTmp);
        //producerTmp->signalProduceMessvoidJson("value", "key1");
    }


    return true;
}

bool MainClass::initControlls()
{

    // QJsonObject lightCfgObj = m_cfgJson.value("light").toObject();

    //获取发送间隔、发送命令数量、上传kafka的主题
    int sendingInterval = m_cfgJson.value("sendingInterval").toString().toInt();
    int sendingCount = m_cfgJson.value("sendingCount").toString().toInt();
    QString topic = m_cfgJson.value("topic").toString();

    // 获取控制器
    QJsonArray controllers = m_cfgJson.value("controllers").toArray();

    for(int i=0; i<controllers.size(); i++){
        QJsonObject controllJson =          // 控制器配置信息
            controllers.at(i).toObject();
        Controller* controll;               // 控制器
        QString ip =                        // 控制器 IP
            controllJson.value("ip").toString();
        int port =                          // 控制器 port
            controllJson.value("port").toString().toInt();
        QString ConnectType =               // 控制器 连接方式
            controllJson.value("connectType").toString().toUpper();
        QString LightListStr =             // 灯id列表  替换 半角 ;   去除 空格
            controllJson.value("lightId").toString().replace(" ", "").replace("；", ";");
        QStringList LightStrId = LightListStr.split(";", Qt::SkipEmptyParts);


        QStringList LightId;
        foreach(QString str, LightStrId){
            QStringList list = str.split("-", Qt::SkipEmptyParts);
            int smallNumbers;
            int bigNumbers;

            if(list.size() == 1){
                smallNumbers = list.at(0).toInt();

                if(smallNumbers < 1 || smallNumbers > 254){
                    return false;
                }

                LightId << QString::number(smallNumbers);
            }else if(list.size() == 2){
                smallNumbers = list.at(0).toInt();
                bigNumbers = list.at(1).toInt();



                if(smallNumbers < 1 || bigNumbers > 254 || smallNumbers >= bigNumbers){
                    return false;
                }

                for(int i=smallNumbers; i<=bigNumbers; i++){
                    LightId << QString::number(i);
                }
            }else{
                return false;
            }
        }

        //创建控制器,并插入控制器列表
        controll = new Controller(ip, port, topic, sendingInterval, sendingCount, ConnectType, this);
        m_controllList.append(controll);

        connect(controll, &Controller::signalWrite2Kafka, this, &MainClass::slotWrite2Kafka);
        controll->start();
        controll->connectController(ConnectType);
        controll->addLights(LightId);
        // emit controll->signalInitTcp(ip, port);
        //emit controll->sigConnectToControl();
    }

    return true;
}

// bool MainClass::initTcpClient()
// {
//     QJsonObject webCfgJson = m_cfgJson.value("web").toObject();
//     QString ip = webCfgJson.value("ip").toString();
//     int port = webCfgJson.value("port").toInt();

//     m_tcpClient = new QTcpSocket;
//     m_tcpClient->connectToHost(ip, port);
//     connect(m_tcpClient, &QTcpSocket::connected, this, [this](){
//         qDebug() <<  "m_tcpClient连接成功";
//     });
//     return true;
// }

void MainClass::slotWrite2Kafka(QString topic, QString strJson, QString strKey)
{
    producer* producer_ = nullptr;

    for(int i=0; i<m_producerList.size(); i++){
        if(m_producerList.at(i)->getTopic() == topic){
            producer_ = m_producerList.at(i);
            break;
        }
    }

    if(producer_){
        emit producer_->signalProduceMessvoidJson(strJson, strKey);
    }
}

void MainClass::slotSetCfgJson(QByteArray cfgJson)
{
    // qDebug() << m_cfgJson;
    // qDebug() << QJsonDocument::fromJson(cfgJson).toJson();

    QJsonObject newCfgJson = QJsonDocument::fromJson(cfgJson).object();
    //qDebug() << newCfgJson.value("deviceStatenh_deviceState")
    QString iniPath = QCoreApplication::applicationDirPath() + CFG_JSON;



    // if(!QFileInfo::exists(iniPath)){
    //     qCritical() << ("****** cfg.json 配置文件丢失 ******");
    // }

    // 打开配置文件
    QFile file(iniPath);
    if(!file.open(QIODevice::WriteOnly)){
        qCritical() << ("****** " + iniPath + " 配置文件打开失败 ******");
    }

    // 写入配置文件
    file.write(cfgJson);


}
