#include "controllerworker.h"
#include "lightCmdList.h"

#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include <QJsonDocument>
#include <QThread>

ControllerWorker::ControllerWorker(QString ip, int port, int sendingInterval, int sendingCount, QString ConnectType, QObject *parent)
    : QObject{parent}
    , m_sendingInterval(sendingInterval)
    , m_sendingCount(sendingCount)
    , m_ConnectType(ConnectType)
    , m_ControllerIp(ip)
    , m_ControllerPort(port)
{

    m_cmdlightRun << "FF 55 01 D3 EA AA" << "FF 55 02 CC EC AA" << "FF 55 03 C2 B7 AA" << "FF 55 04 BB AC AA";
}

void ControllerWorker::boadCast(QStringList cmdList)
{
    QEventLoop eventloop;
    while(cmdList.size() > 0){
        tcpConnect2Host();
        // m_tcpSocket->write(QByteArray::fromHex(cmdList.first().toLatin1()));


        sendCmd(cmdList.first());

        QTimer::singleShot(1000, &eventloop, SLOT(quit()));
        eventloop.exec();
        cmdList.removeFirst();
    }
}

void ControllerWorker::boadCastNot(QStringList cmdList)
{
    while(cmdList.size() > 0){
        QString cmd;
        for(int i = 0; i<m_sendingCount && !cmdList.isEmpty() ; i++){
            cmd += (cmdList.first() + " ");
            cmdList.removeFirst();
        }

        sendCmd(cmd);
        // if(m_ConnectType == "TCP"){
        //     tcpConnect2Host();
        //     m_tcpSocket->write(QByteArray::fromHex(cmd.toLatin1()));
        //     m_tcpSocket->waitForBytesWritten();
        // }else{
        //     m_udpSocket->writeDatagram(QByteArray::fromHex(cmd.toLatin1()), QHostAddress(m_ControllerIp), m_ControllerPort);
        // }
    }
}

int ControllerWorker::getLightIdFromCmd(QString cmd)
{
    QStringList strList= cmd.toUpper().split(" ");
    if(strList.size() != 6) return -1;

    return strList.at(2).toInt(nullptr, 16);
}

void ControllerWorker::initCheckTimer()
{
    m_checkTimer = new QTimer;
    m_checkTimer->setInterval(m_IntervalTime);

    connect(m_checkTimer, &QTimer::timeout, this, [=]{

        if(workMode == 1){              // 控灯模式
            qDebug() << "控灯模式";
            m_lightRunOn = false;
            sendCmd2Controller();
            workMode = 2;

        }else if(workMode == 2){         // 查询模式

            m_lightRunOn = false;
            qDebug() << "m_checkstate" << m_checkstate;
            // 是否在等数据返回
            if(waitForData > 0){

                if(m_ConnectType == "TCP"){
                    slotReadyReadTcp();
                }else{
                    slotReadyReadUdp();
                }
                if(waitForData > 0 && waitForData <= m_IntervalTime * 2){
                    return;
                }else{
                    waitForData = 0;
                    if(m_cmdCheckState.isEmpty() && m_cmdCheckPowerState.isEmpty()
                        && m_cmdCheckPathTrackingDelay.isEmpty() && m_cmdCheckPathTrackingMode.isEmpty()){
                        emit signalWrite2Kafka();
                    }
                }
            }


            // 当前是什么查询状态
            QStringList* cmdList = nullptr;
            QString cmd;
            switch(m_checkstate){
            case 0:
                workMode = 3;
                qDebug() << "no查询";
                break;
            case 1:
                // 需要查1
                if(!m_cmdCheckState.isEmpty()){
                    cmdList = &m_cmdCheckState;
                }
                break;
            case 2:
                // 需要查2
                if(!m_cmdCheckPowerState.isEmpty()){
                    cmdList = &m_cmdCheckPowerState;
                }
                break;
            case 3:
                // 需要查3
                if(!m_cmdCheckPathTrackingDelay.isEmpty()){
                    cmdList = &m_cmdCheckPathTrackingDelay;
                }
                break;
            case 4:
                // 需要查4
                if(!m_cmdCheckPathTrackingMode.isEmpty()){
                    cmdList = &m_cmdCheckPathTrackingMode;
                }
                break;
            case 5:
                if(!m_cmdCheckState.isEmpty()){
                    cmdList = &m_cmdCheckState;
                }else if(!m_cmdCheckPowerState.isEmpty()){
                    cmdList = &m_cmdCheckPowerState;
                }else if(!m_cmdCheckPathTrackingDelay.isEmpty()){
                    cmdList = &m_cmdCheckPathTrackingDelay;
                }
                break;
            default:
                break;
            }

            if(cmdList){
                cmd = cmdList->first();
                cmdList->removeFirst();
                currentCheckLightId = getLightIdFromCmd(cmd);

                sendCmd(cmd);

                qDebug() << (QString("控制器:%1[%2]").arg(m_ControllerIp).arg(cmd));
                waitForData = m_IntervalTime;
            }else{
                m_checkstate = 0;
            }
        }else if(workMode == 3){         // 跑马模式
            if(m_lightRunOn){
                marquee();
            }else{
                workMode = 0;
                qDebug() << "跑马模式关";
            }
        }
    });

    m_checkTimer->start();
}

void ControllerWorker::sendCmd2Controller()
{
    QString tmp;
    QEventLoop eventloop;


    while(!m_cmdList.isEmpty()){
        QString str = m_cmdList.first().toUpper();

        // 将闪烁 和 亮度 和 颜色 命令发送，并休眠 1s
        if(    m_cmdList.first().toUpper().contains("FF 66 FF")
            || m_cmdList.first().toUpper().contains("FF 77 FF")
            || m_cmdList.first().toUpper().contains("FF 88 FF")
            || m_cmdList.first().toUpper().contains("FF 40 FF")
            || m_cmdList.first().toUpper().contains("FF 08 FF")  )
        {
            // if(m_ConnectType == "TCP"){
            //     tcpConnect2Host();
            //     if(m_tcpSocket->state() != QTcpSocket::ConnectedState){
            //         tcpConnect2Host();
            //     }
            //     m_tcpSocket->write(QByteArray::fromHex(m_cmdList.first().toLatin1()));
            //     m_tcpSocket->waitForBytesWritten();
            // }else{
            //     m_udpSocket->writeDatagram(QByteArray::fromHex(m_cmdList.first().toLatin1()), QHostAddress(m_ControllerIp), m_ControllerPort);
            // }

            sendCmd(m_cmdList.first());

            qDebug() << (QString("控制器:%1[%2]").arg(m_ControllerIp).arg(m_cmdList.first()));

            QTimer::singleShot(m_sendingInterval, &eventloop, SLOT(quit()));
            eventloop.exec();
            m_cmdList.removeFirst();
            continue;
        }

        for(int i = 0; i<m_sendingCount && !m_cmdList.isEmpty() ; i++){
            tmp = tmp + m_cmdList.first() + " ";
            m_cmdList.removeFirst();
        }

        sendCmd(tmp);

        // if(m_ConnectType == "TCP"){
        //     tcpConnect2Host();
        //     m_tcpSocket->write(QByteArray::fromHex(tmp.toLatin1()));
        //     m_tcpSocket->waitForBytesWritten();
        // }else{
        //     m_udpSocket->writeDatagram(QByteArray::fromHex(tmp.toLatin1()), QHostAddress(m_ControllerIp), m_ControllerPort);
        // }

        QTimer::singleShot(m_sendingInterval, &eventloop, SLOT(quit()));
        eventloop.exec();
        // if(m_ConnectType == "TCP"){
        //     tcpConnect2Host();
        //     m_tcpSocket->write(QByteArray::fromHex(tmp.toLatin1()));
        //     m_tcpSocket->waitForBytesWritten();
        // }else{
        //     m_udpSocket->writeDatagram(QByteArray::fromHex(tmp.toLatin1()), QHostAddress(m_ControllerIp), m_ControllerPort);
        // }


        sendCmd(tmp);
        qDebug() << (QString("控制器:%1[%2]").arg(m_ControllerIp).arg(tmp));
        tmp.clear();
    }

}

void ControllerWorker::tcpConnect2Host()
{
    if(!m_tcpSocket){
        qDebug() << (QString("控制器:%1:%2[m_tcpSocket不存在]").arg(m_ControllerIp).arg(m_ControllerPort));
        return;
    }

    if(m_tcpSocket->state() == QTcpSocket::ConnectedState){
        return;
    }
    qDebug() << (QString("控制器:%1:%2[正在连接]").arg(m_ControllerIp).arg(m_ControllerPort));
    m_tcpSocket->connectToHost(m_ControllerIp, m_ControllerPort);
    m_tcpSocket->setSocketOption(QTcpSocket::KeepAliveOption, 1);

    for (int i=0; i< 5; i++)  // 连接
    {
        if(m_tcpSocket->waitForConnected(1000)){
            qDebug() << (QString("控制器[%1:%2]连接成功").arg(m_ControllerIp).arg(m_ControllerPort));
            break;
        }else{

            if(i == 4){
                qDebug() << (QString("控制器[%1:%2]连接失败, 放弃连接").arg(m_ControllerIp).arg(m_ControllerPort));
            }else{
                qDebug() << (QString("控制器[%1:%2]连接失败，重试中...").arg(m_ControllerIp).arg(m_ControllerPort));
            }
        }

    }

    return;
}

void ControllerWorker::sendCmd(QString cmd)
{
    // 发送命令
    if(m_ConnectType == "TCP" && m_tcpSocket){
        tcpConnect2Host();
        m_tcpSocket->write(QByteArray::fromHex(cmd.toLatin1()));
        m_tcpSocket->waitForBytesWritten();

        qDebug() << (QString("(TCP)控制器:%1[%2]").arg(m_ControllerIp).arg(cmd));
    }else if(m_ConnectType == "UDP" && m_udpSocket){
        m_udpSocket->writeDatagram(QByteArray::fromHex(cmd.toLatin1()), QHostAddress(m_ControllerIp), m_ControllerPort);
        qDebug() << (QString("(UDP)控制器:%1[%2]").arg(m_ControllerIp).arg(cmd));
    }

}

void ControllerWorker::slotUpdateMarqueeData(QStringList hexContentList)
{
    qDebug() << hexContentList;
    m_cmdlightRun = hexContentList;
}

void ControllerWorker::slotInitWorker()
{
    initCheckTimer();
}

void ControllerWorker::slotInitTcp(QString ip, int port)
{
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, &QTcpSocket::stateChanged, this, [=](QAbstractSocket::SocketState state){
        switch (state) {
        case QAbstractSocket::UnconnectedState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("未连接");
            break;
        case QAbstractSocket::HostLookupState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("查询地址");
            break;
        case QAbstractSocket::ConnectingState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("正在连接");
            break;
        case QAbstractSocket::ConnectedState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("成功连接");
            break;
        case QAbstractSocket::BoundState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("绑定模式");
            break;
        case QAbstractSocket::ListeningState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("监听模式");
            break;
        case QAbstractSocket::ClosingState:
            qDebug() << QString("%1:%2 连接状态: %3").arg(ip).arg(port).arg("断开连接");
            tcpConnect2Host();
            break;
        default:
            break;
        }
    });

    qDebug() << (QString("控制器:%1:%2[正在连接]").arg(ip).arg(port));
    m_tcpSocket->connectToHost(ip, port);
    m_tcpSocket->setSocketOption(QTcpSocket::KeepAliveOption, 1);


    for (int i=0; i< 5; i++)  // 连接
    {
        if(m_tcpSocket->waitForConnected(1000)){
            qDebug() << (QString("控制器[%1:%2]连接成功").arg(ip).arg(port));
            break;
        }else{

            if(i == 4){
                qDebug() << (QString("控制器[%1:%2]连接失败, 放弃连接").arg(ip).arg(port));
            }else{
                qDebug() << (QString("控制器[%1:%2]连接失败，重试中...").arg(ip).arg(port));
            }
        }

    }

    return;
    //connect(m_tcpSocket,SIGNAL(readyRead()), this, SLOT(slotReadyReadTcp()));
    //connect(m_tcpSocket, SIGNAL(disconnected()));
    // connect(m_tcpSocket, &QTcpSocket::stateChanged, this, [this](){
    //     qDebug() << (QString("[%1] stateChanged to %2").arg(m_ControllerIpPort).arg(m_tcpSocket->state()));
    // });
    // connect(m_tcpSocket, &QTcpSocket::disconnected, this, [this](){
    //     qDebug() << (QString("[%1] disconnected!").arg(m_ControllerIpPort));
    // });
}

void ControllerWorker::slotInitUdp()
{
    m_udpSocket = new QUdpSocket(this);

    //connect(m_udpSocket,SIGNAL(readyRead()), this, SLOT(slotReadyReadUdp()));

    //connect(m_tcpSocket, SIGNAL(disconnected()));
    // connect(m_tcpSocket, &QUdpSocket::stateChanged, [this](){
    //     qDebug() << (QString("[%1] stateChanged to %2").arg(m_controllIp).arg(m_tcpSocket->state()));
    // });
    // connect(m_tcpSocket, &QUdpSocket::disconnected, [this](){
    //     qDebug() << (QString("[%1] disconnected!").arg(m_controllIp));
    // });
}

void ControllerWorker::slotReadyReadTcp()
{
    QByteArray ba = m_tcpSocket->readAll().toHex();
    //qDebug() << "*************ba: " << ba;
    QString str;
    QString msg;

    str = ba.toUpper();

    // 没有数据
    if(str.size() != 4 && str.size() != 2){
        waitForData += m_IntervalTime;
        if(waitForData > m_IntervalTime * 2){
            qDebug() << "ip: " << m_ControllerIp << "   id : " << currentCheckLightId << "  状态: 通信失败";
            emit signalLightIsOff(currentCheckLightId);
        }
        return;
    }

    int state;
    if(str.left(2) == "F5"){
        state = QString(str.at(2)).toInt();
        switch (state) {
        case 0:
            msg = "灯关闭";
            emit signalLightIsOff(currentCheckLightId);
            break;
        case 1:
            msg = "灯亮";
            break;
        case 3:
            msg = "灯亮且闪烁";
            break;
        case 4:
            emit signalLightIsOff(currentCheckLightId);
            msg = "内部通讯故障";
            break;
        default:
            break;
        }
        qDebug() << "id : " << currentCheckLightId << "  状态: " << msg;
        waitForData = 0;
    }else if(str.left(2) == "F6"){

    }else if(str.left(2) == "FB"){
        state = str.right(2).toInt(nullptr, 16) / 10;
        qDebug() << "id : " << currentCheckLightId << "  轨迹跟踪延时: " << state;
        waitForData = 0;
        return;
    }

}

void ControllerWorker::slotReadyReadUdp()
{
    // qDebug() << "m_udpSocket->hasPendingDatagrams();";
    //qDebug() << m_udpSocket->readAll();
}

void ControllerWorker::slotCmd2Controller(QStringList cmdList)
{
    m_cmdList = cmdList;
    workMode = 1;
}

void ControllerWorker::slotFlushAutoCheckCmd(QStringList cmdCheckState, QStringList cmdCheckPowerState, QStringList cmdCheckPathTrackingDelay)
{
    m_cmdCheckState = cmdCheckState;
    m_cmdCheckPowerState = cmdCheckPowerState;
    m_cmdCheckPathTrackingDelay = cmdCheckPathTrackingDelay;
}

void ControllerWorker::slotSendControlCmd(QStringList cmdList)
{
    m_cmdList = cmdList;
    workMode = 1;
}

void ControllerWorker::slotSendCheckCmd(QStringList cmdList)
{
    m_cmdCheckState = cmdList;
    m_checkstate = 1;
    // workMode = 1;
}

void ControllerWorker::slotLightPowerOn(bool on)
{
    if(m_lightPowerOn == on) return;

    m_lightPowerOn = on;

    // 灯柱电源开关
    QString cmd = on ? CMD_POWER_ON : CMD_POWER_OFF;

    if(on){
        //qDebug() << QString("开电时间:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"));
        qDebug() << (QString("[%1]  控制器:%2[%3]  开电").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(m_ControllerIp).arg(cmd));
    }else{
        //qDebug() << QString("关电时间:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"));
        qDebug() << (QString("[%1]  控制器:%2[%3]  关电").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(m_ControllerIp).arg(cmd));
    }

    if(m_ConnectType == "TCP"){
        tcpConnect2Host();
        m_tcpSocket->write(QByteArray::fromHex(cmd.toLatin1()));
        m_tcpSocket->waitForBytesWritten();
    }else{
        m_udpSocket->writeDatagram(QByteArray::fromHex(cmd.toLatin1()), QHostAddress(m_ControllerIp), m_ControllerPort);
    }


    QThread::msleep(500);
}

void ControllerWorker::slotOpenMarquee(bool open)
{
    if(open){
        workMode = 3;
    }
    m_lightRunOn = open;
}

void ControllerWorker::slotUpdateLisghtIds(QStringList lightIds)
{
    m_lightIds = lightIds;
}

void ControllerWorker::marquee()
{

    if(m_headIndex == m_marqueeTail + m_marqueeHead){
        m_headIndex = 0;
    }

    QString cmdList;    // 命令列表
    int index = 0;  // 访问 m_cmdlightRun

    for(int i = 0; i<m_marqueeHead-m_headIndex; i++, index++){
        if(index >= m_cmdlightRun.size()) break;
        cmdList += m_cmdlightRun.at(index);
        cmdList += " ";
    }

    for(int i = 0; i<m_marqueeTail; i++, index++){
        if(index >= m_cmdlightRun.size()) break;
        continue;
    }

    while(index < m_cmdlightRun.size()){

        for(int i = 0; i<m_marqueeHead; i++, index++){
            if(index >= m_cmdlightRun.size()) break;
            cmdList += m_cmdlightRun.at(index);
            cmdList += " ";
        }

        if(index >= m_cmdlightRun.size()) break;
        for(int i = 0; i<m_marqueeTail; i++, index++){
            if(index >= m_cmdlightRun.size()) break;
            continue;
        }
    }

    sendCmd(CLOSE_LIGHT);
    sendCmd(cmdList);

    m_headIndex++;
}
