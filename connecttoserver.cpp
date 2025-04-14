#include "connecttoserver.h"

connectToServer::connectToServer(QString serverI, int serverPort, QObject *parent)
    : QObject{parent}
    , m_serverIp(serverI)
    , m_serverPort(serverPort)
{

    initLightClient();

}

void connectToServer::initLightClient()
{
    m_lightClient = new QTcpSocket;
    connect(m_lightClient, &QTcpSocket::disconnected, this, [this](){
        if(m_lightClient){
            m_lightClient->connectToHost(m_serverIp, m_serverPort);
        }
    });
}
