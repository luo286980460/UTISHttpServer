#include "mainclass.h"
#include "myhttpserver.h"
#include "producer.h"

#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>

#define INIFILE_KAFKA        "/kafka.ini"

MainClass::MainClass(QObject *parent)
    : QObject{parent}
{
    init();
}

void MainClass::init()
{
    m_myHttpServer = new MyHttpServer(this);

    initProducer();
}

void MainClass::initProducer()
{
    QString iniPath = QCoreApplication::applicationDirPath() + INIFILE_KAFKA;
    if(!QFileInfo::exists(iniPath)){
        qDebug() << ("****** kafka.ini 配置文件丢失 ******");
        return;
    }
    producer* producerTmp = nullptr;
    QSettings settings(iniPath, QSettings::IniFormat);
    int topicCount = settings.value("topics/topicCount", 0).toInt();
    QString kafkaIp = settings.value("kafka/ip", 0).toString();

    for(int i=0; i<topicCount; i++){
        QString strTopic = settings.value(QString("topics/topic%1").arg(i+1,2,10,QLatin1Char('0')), "test_ls").toString(); //kafka主题
        producerTmp = new producer(this);
        producerTmp->setTopic(strTopic);
        producerTmp->setkafkaIp(kafkaIp);
        producerTmp->start();
        producerTmp->init();
        emit producerTmp->signalInitWork();
        m_producerList.append(producerTmp);

        producerTmp->signalProduceMessvoidJson("value", "key1");
    }
}
