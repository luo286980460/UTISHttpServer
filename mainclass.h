#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>

class MyHttpServer;
class producer;

class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QObject *parent = nullptr);

    void init();
    void initProducer();

private:

signals:

public slots:

private:
    MyHttpServer* m_myHttpServer;

    // kafka模块
    QList<producer*> m_producerList; // kafka创造者列表
};

#endif // MAINCLASS_H
