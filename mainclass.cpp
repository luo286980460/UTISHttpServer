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
    if(!initCfgJson()) return;
    if(!initProducer()) return;
    if(!initControlls()) return;
    if(!initHttpserver()) return;

    // 连接信号
    // conn
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
                    .object()
                    .value("utisDeviceServer")
                    .toObject();

    if(error.error != QJsonParseError::NoError){
        qCritical() << "error cfgJson: " + error.errorString();
        return false;
    }

    // 子参数 light/kafka 是否为json
    if(!m_cfgJson.value("light").isObject() || !m_cfgJson.value("kafka").isObject()){
        qCritical() << "light/kafka not a json";
        return false;
    }

    return true;
}

bool MainClass::initHttpserver()
{
    int port = m_cfgJson.value("port").toInt();

    if(port < 1) {
        qCritical() << " error UtisDeviceServer Port = " + QString::number(port);
        return false;
    }

    m_myHttpServer = new MyHttpServer(port, this);
    connect(m_myHttpServer, &MyHttpServer::signalWrite2Kafka, this, &MainClass::slotWrite2Kafka);


    m_myHttpServer->updateControllList(&m_controllList);
    return true;
}

bool MainClass::initProducer()
{
    producer* producerTmp = nullptr;
    QJsonObject kafkaCfgObj = m_cfgJson.value("kafka").toObject();
    QString kafkaIp = kafkaCfgObj.value("ip").toString();
    QJsonArray topicList = kafkaCfgObj.value("topics").toArray();

    for(int i=0; i<topicList.size(); i++){
        if(!topicList.at(i).isString()){
            qWarning() << " error kafka topic is not string";
            return false;
        }
        QString strTopic = topicList.at(i).toString(); //kafka主题
        producerTmp = new producer(this);
        //connect(this, &MainClass::write2Kafka, producerTmp, &producer::slotProduceMessvoidJson);
        producerTmp->setTopic(strTopic);
        producerTmp->setkafkaIp(kafkaIp);
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

    QJsonObject lightCfgObj = m_cfgJson.value("light").toObject();

    //获取发送间隔、发送命令数量、上传kafka的主题
    int sendingInterval = lightCfgObj.value("sendingInterval").toInt();
    int sendingCount = lightCfgObj.value("sendingCount").toInt();
    QString topic = lightCfgObj.value("topic").toString();

    // 获取控制器
    QJsonArray controllers = lightCfgObj.value("controllers").toArray();

    for(int i=0; i<controllers.size(); i++){
        QJsonObject controllJson =          // 控制器配置信息
            controllers.at(i).toObject();
        Controller* controll;               // 控制器
        QString ip =                        // 控制器 IP
            controllJson.value("ip").toString();
        int port =                          // 控制器 port
            controllJson.value("port").toInt();
        QString ConnectType =               // 控制器 连接方式
            controllJson.value("connectType").toString().toUpper();
        QJsonArray LightArray =             // 灯id列表
            controllJson.value("lightId").toArray();
        QStringList LightId;                // 灯id列表connectType

        for(int i=0; i<LightArray.size(); i++){
            LightId << QString::number(LightArray.at(i).toInt());
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
