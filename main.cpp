#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>

#include "mainclass.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif


// 全局文件对象和互斥锁（确保多线程安全）
static QFile s_logFile;
static QMutex s_mutex;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    // 隐藏控制台窗口
    #ifdef Q_OS_WIN
        FreeConsole(); // 隐藏控制台窗口
    #endif

    // 防止程序多次启动
    // 尝试创建共享内存段
    // QSharedMemory sharedMem("MyAppLock");
    // if (sharedMem.attach()) {
    //     // 已存在实例，退出
    //     return 0;
    // }
    // sharedMem.create(1);

    // // 启动本地服务器监听新实例请求
    // QLocalServer server;
    // server.listen("MyAppServer");

    QCoreApplication a(argc, argv);

    // 安装自定义消息处理函数
    //qInstallMessageHandler(messageHandler);

    MainClass mainClass;

    return a.exec();
}


void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&s_mutex); // 加锁防止多线程竞争

    // 打开日志文件（如果未打开）
    if (!s_logFile.isOpen()) {
        QString logDirPath = QCoreApplication::applicationDirPath() + "/log";
        QString logFilePath = logDirPath + "/qdebugLog.txt";

        if(!QDir(logDirPath).exists()){
            QDir dir;
            dir.mkpath(logDirPath);
        }

        s_logFile.setFileName(logFilePath); // 日志文件名
        s_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    // 格式化日志信息
    QString logEntry = QString("[%1] %2: %3\n")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                           .arg(qFormatLogMessage(type, context, msg)) // 类型和消息
                           .arg(context.function ? context.function : ""); // 函数名（可选）

    // 写入文件
    QTextStream stream(&s_logFile);
    stream << logEntry;
    stream.flush(); // 确保立即写入
}
