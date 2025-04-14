#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QObject>
#include <QTcpSocket>

class connectToServer : public QObject
{
    Q_OBJECT
public:
    explicit connectToServer(QString serverI, int serverPort, QObject *parent = nullptr);

private:
    void initLightClient();

signals:

private:
    QTcpSocket* m_lightClient;
    QString m_serverIp;
    int m_serverPort;
};

#endif // CONNECTTOSERVER_H
