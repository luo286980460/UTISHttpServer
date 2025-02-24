#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>

#include "myhttpserver.h"
#include "controller.h"
#include "include/libhv/hthread.h"    // import hv_gettid
#include "include/libhv/hasync.h"     // import hv::async
#include "lightCmdList.h"

using namespace hv;

MyHttpServer::MyHttpServer(int port, QObject *parent)
    : QObject(parent)
{
    createHttpserver(port);
}

MyHttpServer::~MyHttpServer()
{
    hv::async::cleanup();
}

void MyHttpServer::updateControllList(QList<Controller *> * controllList)
{
    m_controllList = controllList;
}

void MyHttpServer::createHttpserver(int port)
{
    HV_MEMCHECK;

    m_router = new HttpService;

    /* Static file service */
    // curl -v http://ip:port/
    m_router->Static("/", "./html");

    /* Forward proxy service */
    m_router->EnableForwardProxy();
    // curl -v http://httpbin.org/get --proxy http://127.0.0.1:8080
    m_router->AddTrustProxy("*httpbin.org");

    /* Reverse proxy service */
    // curl -v http://ip:port/httpbin/get
    m_router->Proxy("/httpbin/", "http://httpbin.org/");

    // 警示灯
    // 广播控灯
    m_router->POST("/light/Broadcast", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        if(QString::fromStdString(req->GetHeader("Content-Type")) != "application/json"){
            QJsonObject jsonBack;
            jsonBack.insert("code", 1);
            jsonBack.insert("msg", "Content-Type must be application/json");
            return resp->String(QJsonDocument(jsonBack).toJson().toStdString());
        }


        return resp->String(QJsonDocument(parseLightBroadcast(jsonObj)).toJson().toStdString());
    });


    // 非广播控灯
    m_router->POST("/light/BroadcastNot", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        if(QString::fromStdString(req->GetHeader("Content-Type")) != "application/json"){
            QJsonObject jsonBack;
            jsonBack.insert("code", 1);
            jsonBack.insert("msg", "Content-Type must be application/json");
            return resp->String(QJsonDocument(jsonBack).toJson().toStdString());
        }

        return resp->String(QJsonDocument(parseLightBroadcastNot(jsonObj)).toJson().toStdString());
    });

    // 雾灯轨迹开关
    m_router->POST("/light/PathTracking", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        if(QString::fromStdString(req->GetHeader("Content-Type")) != "application/json"){
            QJsonObject jsonBack;
            jsonBack.insert("code", 1);
            jsonBack.insert("msg", "Content-Type must be application/json");
            return resp->String(QJsonDocument(jsonBack).toJson().toStdString());
        }

        return resp->String(QJsonDocument(parseLightPathTracking(jsonObj)).toJson().toStdString());
    });

    // 雾灯状态更新
    m_router->POST("/light/UpdateLightState", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        if(QString::fromStdString(req->GetHeader("Content-Type")) != "application/json"){
            QJsonObject jsonBack;
            jsonBack.insert("code", 1);
            jsonBack.insert("msg", "Content-Type must be application/json");
            return resp->String(QJsonDocument(jsonBack).toJson().toStdString());
        }

        return resp->String(QJsonDocument(parseUpdateLightState(jsonObj)).toJson().toStdString());
    });


    // 旧版控灯协议 2.0
    m_router->POST("/light", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        //qDebug()<< jsonObj;

        return resp->String(QJsonDocument(parseLightJson(jsonObj)).toJson().toStdString());
    });




    /*          POST            */
    // curl -v http://ip:port/echo -d "hello,world!"
    m_router->POST("/echo", [](const HttpContextPtr& ctx) {
        return ctx->send(ctx->body(), ctx->type());
    });

    /*          GET            */
    /* API handlers */
    // curl -v http://ip:port/ping
    m_router->GET("/ping", [](HttpRequest* req, HttpResponse* resp) {
        Q_UNUSED(req);
        Json ex3 =  {
            {"time", "最后更新时间：2025年02月24日"},
            {"Name", "尤特斯设备服务"},
            {"Version", "0.5"},
            {"Msg", "支持警示灯新版，有kafka，此版本还在测试中，后续可能修改功能以及协议"}
        };
        return resp->Json(ex3);
        //return resp->String("connected............");
    });

    // curl -v http://ip:port/paths
    m_router->GET("/paths", [this](HttpRequest* req, HttpResponse* resp) {
        Q_UNUSED(req);
        return resp->Json(m_router->Paths());
    });

    // curl -v http://ip:port/get?env=1
    m_router->GET("/get", [](const HttpContextPtr& ctx) {
        hv::Json resp;
        resp["origin"] = ctx->ip();
        resp["url"] = ctx->url();
        resp["args"] = ctx->params();
        resp["headers"] = ctx->headers();
        return ctx->send(resp.dump(2));
    });

    // curl -v http://ip:port/user/123
    m_router->GET("/user/{id}", [](const HttpContextPtr& ctx) {
        hv::Json resp;
        resp["id"] = ctx->param("id");
        return ctx->send(resp.dump(2));
    });

    // curl -v http://ip:port/async
    m_router->GET("/async", [](const HttpRequestPtr& req, const HttpResponseWriterPtr& writer) {
        Q_UNUSED(req);
        writer->Begin();
        writer->WriteHeader("X-Response-tid", hv_gettid());
        writer->WriteHeader("Content-Type", "text/plain");
        writer->WriteBody("This is an async response.\n");
        writer->End();
    });

    m_httpServer = new HttpServer;
    m_httpServer->service = m_router;
    m_httpServer->port = port;

#if TEST_HTTPS
    server.https_port = 8443;
    hssl_ctx_opt_t param;
    memset(&param, 0, sizeof(param));
    param.crt_file = "cert/server.crt";
    param.key_file = "cert/server.key";
    param.endpoint = HSSL_SERVER;
    if (server.newSslCtx(&param) != 0) {
        fprintf(stderr, "new SSL_CTX failed!\n");
        return -20;
    }
#endif

    // uncomment to test multi-processes
    // server.setProcessNum(4);
    // uncomment to test multi-threads
    // server.setThreadNum(4);


    m_httpServer->start();
}

bool MyHttpServer::ipAddrIsOK(const QString &ip)
{
    if (ip.isEmpty())
    {
        return false;
    }

    QStringList list = ip.split('.');
    if (list.size() != 4)
    {
        return false;
    }

    for (const auto& num : list)
    {
        bool ok = false;
        int temp = num.toInt(&ok);
        if (!ok || temp < 0 || temp > 255)
        {
            return false;
        }
    }

    return true;
}

QString MyHttpServer::qstr2Hex(QString instr)
{
    if(instr.isEmpty()) return instr;

    QTextCodec *codec = QTextCodec::codecForName("GB2312");  //set char code
    QByteArray encodedString = codec->fromUnicode(instr);//convert to unicode
    instr = encodedString.toHex(QChar::Space); //hex add space example:FFAC21 convert to FF AC 21
    return instr.toUpper();
}

bool MyHttpServer::missingParameter(QJsonObject &json, QJsonObject &backJson)
{
    if(json.find("TermIp") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 TermIp ";
        return true;
    }else if(!json.find("TermIp")->isString()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "TermIp 数据类型错误 应该为 string";
        return true;
    }else if(json["TermIp"].toString().split(":").size() != 2){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ip地址不合法,格式范例：192.168.1.186:8886";
        return true;
    }

    if(json.find("TermId") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 TermId ";
        return true;
    }else if(!json.find("TermId")->isArray()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "TermId 数据类型错误  应该为 int 类型的 array";
        return true;
    }else {
        QJsonArray TermId = json["TermId"].toArray();
        int TermIdSize = TermId.size();

        if(TermIdSize < 1){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "TermId 数量不能小于1";
            return true;
        }

        foreach(QJsonValue value, TermId){
            if(!value.isDouble()){
                backJson.find("code").value() = 1;
                backJson.find("msg").value() = "TermId 内容不是 int";
                return true;
            }
        }
    }

    if(json.find("Content") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 Content ";
        return true;
    }else if(!json.find("Content")->isArray()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Content 数据类型错误 应该为 string 类型的 array";
        return true;
    }else{
        QJsonArray Content = json["Content"].toArray();
        foreach(QJsonValue value, Content){
            if(!value.isString()){
                backJson.find("code").value() = 1;
                backJson.find("msg").value() = "content Array 里面的部分数据类型不是 string";
                return true;
            }else if(value.toString().size() != 1){
                backJson.find("code").value() = 1;
                backJson.find("msg").value() = "content Array 里面的数据内容不是 单个文字";
                return true;
            }
        }
    }

    if(json.find("Version") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 Version ";
        return true;
    }else if(!json.find("Version")->isDouble()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Version 数据类型错误 应该为 int";
        return true;
    }

    if(json.find("DeviceId") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 DeviceId ";
        return true;
    }else if(!json.find("DeviceId")->isString()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "DeviceId 数据类型错误 应该为 string";
        return true;
    }

    if(json.find("Broadcast") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 Broadcast ";
        return true;
    }else if(!json.find("Broadcast")->isDouble()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Broadcast 数据类型错误 应该为 int";
        return true;
    }

    if(json.find("name") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 name ";
        return true;
    }else if(!json.find("name")->isString()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "name 数据类型错误 应该为 string";
        return true;
    }

    return false;
}

bool MyHttpServer::missingParameterBroadcast(QJsonObject &json, QJsonObject &backJson)
{
    // 控制器 ip:port
    if(json.find("ControllerIpPort") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 ControllerIpPort ";
        return true;
    }else if(!json.value("ControllerIpPort").isString()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ControllerIpPort 数据类型错误 应该为 string";
        return true;
    }else if(json.value("ControllerIpPort").toString().split(":").size() != 2){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ControllerIpPort 不合法,格式范例:192.168.1.186:8886";
        return true;
    }


    // 控制器 设备编号
    // if(json.find("DeviceId") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 DeviceId ";
    //     return true;
    // }else if(!json.value("DeviceId").isString()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "DeviceId 数据类型错误 应该为 string";
    //     return true;
    // }

    // 文字 内容
    // if(json.find("Content") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 Content ";
    //     return true;
    // }else if(!json.value("Content").isString()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Content 数据类型错误 应该为 string";
    //     return true;
    // }else if(json.value("Content").toString().size() == 0){
    //     return false;
    // }else if(json.value("Content").toString().size() != 1){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Content值 数量只能为1/0";
    //     return true;
    // }

    // 灯 版本
    if(json.find("Version") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 Version ";
        return true;
    }else if(!json.value("Version").isDouble()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Version 数据类型错误 应该为 int";
        return true;
    }else if(json.value("Version").toInt() != 1 && json.value("Version").toInt() != 2 ){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Version 值 应该为 1或者2";
        return true;
    }

    // 文字 颜色
    // if(version == 2){
    //     if(json.find("FontColor") == json.end()) {
    //         backJson.find("code").value() = 1;
    //         backJson.find("msg").value() = "version为2时 缺少必要参数 FontColor ";
    //         return true;
    //     }else if(!json.value("FontColor").isDouble()){
    //         backJson.find("code").value() = 1;        if(json.find("FontColor") == json.end()) {
    //             backJson.find("code").value() = 1;
    //             backJson.find("msg").value() = "缺少必要参数 FontColor ";
    //             return true;
    //         }else if(!json.value("FontColor").isDouble()){
    //             backJson.find("code").value() = 1;
    //             backJson.find("msg").value() = "FontColor 数据类型错误 应该为 int";
    //             return true;
    //         }else if(json.value("FontColor").toInt() < 1 || json.value("FontColor").toInt() > 9 ){
    //             backJson.find("code").value() = 1;
    //             backJson.find("msg").value() = "FontColor 应该为 1-9";
    //             return true;
    //         }
    //         backJson.find("msg").value() = "FontColor 数据类型错误 应该为 int";
    //         return true;
    //     }else if(json.value("FontColor").toInt() < 1 || json.value("FontColor").toInt() > 9 ){
    //         backJson.find("code").value() = 1;
    //         backJson.find("msg").value() = "FontColor 应该为 1-9";
    //         return true;
    //     }
    // }

    // 灯 亮度
    // if(json.find("Luminance") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 Luminance ";
    //     return true;
    // }else if(!json.value("Luminance").isDouble()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Luminance 数据类型错误 应该为 int";
    //     return true;
    // }else if(json.value("Luminance").toInt() < 1 || json.value("Luminance").toInt() > 100 ){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Luminance 应该为 1-100";
    //     return true;
    // }

    // // 灯 轨迹模式
    // if(json.find("PathTracking") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 PathTracking ";
    //     return true;
    // }else if(!json.value("PathTracking").isDouble()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "PathTracking 数据类型错误 应该为 int";
    //     return true;
    // }else if(json.value("PathTracking").toInt() < 0 || json.value("PathTracking").toInt() > 2 ){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "PathTracking 应该为 0-2";
    //     return true;
    // }

    // // 灯 轨迹延时
    // if(json.find("PathTrackingTime") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 PathTrackingTime ";
    //     return true;
    // }else if(!json.value("PathTrackingTime").isDouble()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "PathTrackingTime 数据类型错误 应该为 int";
    //     return true;
    // }else if(json.value("PathTrackingTime").toInt() < 1 || json.value("PathTrackingTime").toInt() > 20 ){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "PathTrackingTime 应该为 0-2";
    //     return true;
    // }

    // 灯 闪烁(size == 0时，为不闪烁）
    // QJsonArray FlickerArray = json.value("Flicker").toArray();
    // if(json.find("Flicker") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 Flicker ";
    //     return true;
    // }else if(!json.value("Flicker").isArray()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Flicker 数据类型错误 应该为 array";
    //     return true;
    // }else if(FlickerArray.size() != 2){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Flicker 数据数量错误 应该为 2个int";
    //     return true;
    // }else if(!FlickerArray.at(0).isDouble() || !FlickerArray.at(1).isDouble()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Flicker内部数据 数据类型错误 应该为 int";
    //     return true;
    // }else if(FlickerArray.at(0).toInt() < 250 || FlickerArray.at(0).toInt() > 10000
    //            || FlickerArray.at(1).toInt() < 250 || FlickerArray.at(1).toInt() > 10000){

    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Flicker内部数据 数据类值错误 应该为 250 - 10000";
    //     return true;
    // }

    return false;

}

QJsonObject MyHttpServer::parseLightBroadcast(QJsonObject &json)
{

    QJsonObject backJson;
    backJson.insert("code", 0);
    backJson.insert("msg", "成功");

    //如果必要参数不存在，或者不合理，直接返回
    if(missingParameterBroadcast(json, backJson)){
        return backJson;
    }
    int version = json.value("Version").toInt();
    QStringList sendDataList;                       // 需要发送的命令列表
    QString ControllerIpPort                                // 控制器 ip:port
        = json["ControllerIpPort"].toString();
    QString ip = ControllerIpPort.split(":").at(0);         // 控制器 ip
    int port = ControllerIpPort.split(":").at(1).toInt();   // 控制器 port
    Controller* controller = getControllerFromIpPort(ip, port); // 控制器

    QString cmdStr; // 发送给雾灯的命令
    // 如果需要改文字 内容
    if(json.find("Content") != json.end()) {
        QString content;
        int contentSize;
        cmdStr = QString(DISPLAY_FONT).replace("%1", "FF");     // 需要发送的命令

        if(!json.value("Content").isString()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Content 数据类型错误 应该为 string";
            return backJson;
        }

        content = json.value("Content").toString();
        contentSize = content.size();

        if(contentSize != 0 && contentSize != 1){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Content值 数量只能为1/0";
            return backJson;
        }

        if(json.value("Content").toString().size() == 1){     // 亮灯
            // 打开电源
            emit controller->signalLightPowerOn(true);

            cmdStr.replace("%2", qstr2Hex(content));
        }else{                                              // 灭灯
            // 关闭电源
            emit controller->signalLightPowerOn(false);

            cmdStr.replace("%2", "A0 F0");
        }

        sendDataList.insert(0, cmdStr.toUpper());
        cmdStr.clear();
    }

    // 如果需要改 颜色
    if(json.find("FontColor") != json.end()) {

        if(!json.value("FontColor").isDouble()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "FontColor 类型应该为 int";
            return backJson;
        }

        int fontColor = json.value("FontColor").toInt();
        if(fontColor < 1 || fontColor > 9) {          // 亮度值不合法
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "FontColor值不合法,应该为 1 - 9";
            return backJson;
        }

        cmdStr = CHANGE_LIGHT_COLOR;     // 需要发送的命令
        cmdStr = QString(cmdStr).arg(fontColor,2,16,QLatin1Char('0'));
        cmdStr.replace("%2", "00");


        if(version == 1){
            backJson.find("code").value() = 0;
            backJson.find("msg").value() = "version 类型为1 不支持修改颜色， 其他命令正常执行";
        }else{
            sendDataList.insert(0, cmdStr.toUpper());
        }
    }

    // 如果需要改 亮度
    if(json.find("Luminance") != json.end()){
        if(!json.value("Luminance").isDouble()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Luminance 类型应该为 int";
            return backJson;
        }

        int luminance = json.value("Luminance").toInt();
        if(luminance < 1 || luminance > 100) {          // 亮度值不合法
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Luminance,应该为 1 - 100";
            return backJson;
        }

        cmdStr = CMD_LUMINANCE;
        sendDataList.insert(0, QString(cmdStr).arg(luminance, 2, 16, QLatin1Char('0')).toUpper());
    }

    // 如果需要改 闪烁
    if(json.find("Flicker") != json.end()){
        if(!json.value("Flicker").isArray()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Flicker 内的数据应该为 int类型的Array";
            return backJson;
        }

        QJsonArray flickerJsonArray = json.value("Flicker").toArray();
        if(flickerJsonArray.size() != 0 && flickerJsonArray.size() != 2){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Flicker内Array的size应该为0或者2";
            return backJson;
        }

        cmdStr = CMD_FLICKER;
        int sec1 = flickerJsonArray.at(0).toInt();
        int sec2 = flickerJsonArray.at(1).toInt();

        // 关闭闪烁
        if(sec1 == 0 || sec2 == 0 || flickerJsonArray.size() == 0){
            sendDataList.insert(0, cmdStr.arg(16,2,16,QLatin1Char('0'))
                                       .arg(0,2,16,QLatin1Char('0')).toUpper());
        }else{  // 闪烁
            if(version == 1){
                if(sec1 < 1000 || sec1 > 10000 || sec2 < 1000 || sec2 > 10000){
                    backJson.find("code").value() = 1;
                    backJson.find("msg").value() = "Version1 闪烁值不合法 1000~10000";
                    return backJson;
                }
                sendDataList.insert(0, QString("ff 77 ff %1 %2 aa")
                                           .arg(QString::asprintf("%.0f", sec1*0.45).toInt()/100,2,16,QLatin1Char('0'))
                                           .arg(QString::asprintf("%.0f", sec2*1.18).toInt()/100,2,16,QLatin1Char('0')));
            }else{
                if(sec1 < 250 || sec1 > 10000 || sec2 < 250 || sec2 > 10000){
                    backJson.find("code").value() = 1;
                    backJson.find("msg").value() = "Version2 闪烁值不合法 250~10000";
                    return backJson;
                }
                sendDataList.insert(0, cmdStr.arg(sec1/20,2,16,QLatin1Char('0'))
                                           .arg(sec2/20,2,16,QLatin1Char('0')).toUpper());
            }
        }
    }

    if(!controllerIsUseful(controller, ControllerIpPort, backJson)){
        return backJson;
    }


    qDebug() << " sendDataList " << sendDataList;
    // 发送命令
    controller->signalSendControlCmd(sendDataList);
    return backJson;
}

bool MyHttpServer::missingParameterBroadcastNot(QJsonObject &json, QJsonObject &backJson)
{
    // 控制器 ip:port
    if(json.find("ControllerIpPort") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 ControllerIpPort ";
        return true;
    }else if(!json.value("ControllerIpPort").isString()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ControllerIpPort 数据类型错误 应该为 string";
        return true;
    }else if(!ipPortIsOK(json.value("ControllerIpPort").toString())){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ControllerIpPort 不合法,格式范例:192.168.1.186:8886";
        return true;
    }

    // 控制器 设备编号
    // if(json.find("DeviceId") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 DeviceId ";
    //     return true;
    // }else if(!json.value("DeviceId").isString()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "DeviceId 数据类型错误 应该为 string";
    //     return true;
    // }

    // 灯 版本
    // if(json.find("Version") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 Version ";
    //     return true;
    // }else if(!json.value("Version").isDouble()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Version 数据类型错误 应该为 int";
    //     return true;
    // }else if(json.value("Version").toInt() != 1 && json.value("Version").toInt() != 2 ){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Version 值 应该为 1或者2";
    //     return true;
    // }

    // 灯 具体数据
    QJsonArray lightsArray = json.value("Lights").toArray();
    if(json.find("Lights") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 Lights ";
        return true;
    }else if(!json.value("Lights").isArray()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Lights 数据类型错误 应该为 array";
        return true;
    }

    foreach(QJsonValue light, lightsArray){
        QJsonObject jsonLight = light.toObject();

        if(!light.isObject()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 格式错误 应该为 json";
            return true;
        }

        if(jsonLight.find("LightId") == jsonLight.end()) {
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 缺少必要参数 LightId ";
            return true;
        }else if(!jsonLight.value("LightId").isDouble()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 LightId 数据类型错误 应该为 int";
            return true;
        }else if(jsonLight.value("LightId").toInt() < 1 || jsonLight.value("LightId").toInt() > 254 ){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 LightId值 应该为 1-254";
            return true;
        }

        if(jsonLight.find("Content") == jsonLight.end()) {
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 缺少必要参数 Content ";
            return true;
        }else if(!jsonLight.value("Content").isString()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 Content 数据类型错误 应该为 string";
            return true;
        }else if(jsonLight.value("Content").toString().size() != 1 ){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Lights内部数据 Content值 数量只能为1";
            return true;
        }

        // if(jsonLight.find("FontColor") == jsonLight.end()) {
        //     backJson.find("code").value() = 1;
        //     backJson.find("msg").value() = "Lights内部数据 缺少必要参数 FontColor ";
        //     return true;
        // }else if(!jsonLight.value("FontColor").isDouble()){
        //     backJson.find("code").value() = 1;
        //     backJson.find("msg").value() = "Lights内部数据 FontColor 数据类型错误 应该为 int";
        //     return true;
        // }else if(jsonLight.value("FontColor").toInt() < 1 || jsonLight.value("FontColor").toInt() > 9 ){
        //     backJson.find("code").value() = 1;
        //     backJson.find("msg").value() = "Lights内部数据 FontColor 应该为 1-9";
        //     return true;
        // }
    }

    return false;
}

QJsonObject MyHttpServer::parseLightBroadcastNot(QJsonObject &json)
{
    QJsonObject backJson;
    backJson.insert("code", 0);
    backJson.insert("msg", "成功");


    //如果必要参数不存在，或者不合理，直接返回
    if(missingParameterBroadcastNot(json, backJson)){
        return backJson;
    }

    QString ControllerIpPort = json["ControllerIpPort"].toString();     // 获取控制器 ip:port
    QString ip = ControllerIpPort.split(":").at(0);                     // 控制器 ip
    int port = ControllerIpPort.split(":").at(1).toInt();               // 控制器 port
    Controller* controller = nullptr;
    QJsonArray lightArray = json.value("Lights").toArray();             // 灯列表
    QStringList sendDataList;                                           // 需要发送的命令列表
    QString cmdStr = DISPLAY_FONT;

    // 获取控制器
    controller = getControllerFromIpPort(ip, port);

    if(!controllerIsUseful(controller, ControllerIpPort, backJson)){
        return backJson;
    }

    for(int i=0; i<lightArray.size(); i++){
        QJsonObject lightJson = lightArray.at(i).toObject();
        sendDataList.append(QString(cmdStr)
                                .arg(lightJson.value("LightId").toInt(), 2, 16, QLatin1Char('0'))
                                .arg(qstr2Hex(lightJson.value("Content").toString())).toUpper());
    }

    // 打开电源
    emit controller->signalLightPowerOn(true);
    controller->signalSendControlCmd(sendDataList);

    return backJson;
}

bool MyHttpServer::ipPortIsOK(QString ipPort)
{
    QString ip = ipPort.split(":").at(0);
    int port = ipPort.split(":").at(1).toInt();

    if (ip.isEmpty())
    {
        return false;
    }

    QStringList list = ip.split('.');
    if (list.size() != 4)
    {
        return false;
    }

    for (const auto& num : list)
    {
        bool ok = false;
        int temp = num.toInt(&ok);
        if (!ok || temp < 0 || temp > 255)
        {
            return false;
        }
    }

    if(port < 1 || port > 65535){    // 判断端口是否合法
        return false;
    }

    return true;
}

QJsonObject MyHttpServer::parseLightJson(QJsonObject &json)
{
    QJsonObject backJson;
    backJson.insert("code", 0);
    backJson.insert("msg", "成功");

    //如果必要参数不存在，或者不合理，直接返回
    if(missingParameter(json, backJson)){
        return backJson;
    }

    QString TermIp = json["TermIp"].toString();     // 获取控制器 ip:port
    QString ip = TermIp.split(":").at(0);           // 获取控制器 ip
    int port = TermIp.split(":").at(1).toInt();     // 获取控制器 port
    QString name = json["name"].toString();         // 获取 name
    QString DeviceId = json["DeviceId"].toString(); // 获取 DeviceId
    int Broadcast = json["Broadcast"].toInt();      // 获取 Broadcast
    QJsonArray TermId = json["TermId"].toArray();   // 获取雾灯编号 TermId
    int TermIdSize = TermId.size();                 // 雾灯编号数量
    int FontColor = -1;                             // 文字颜色
    int Luminance = -1;                             // 亮度值
    int Version = json["Version"].toInt();          // 获取版本号
    QJsonArray Content = json["Content"].toArray(); // 文字内容 array
    QString ContentStr;                             // 实际发送内容的字符串
    int contentSize = Content.size();               // 文字内容数量
    QJsonArray Flicker;                             // 闪烁值列表
    QString FlickerList = "no";                     // 闪烁值字符串
    QStringList sendDataList;                       // 需要发送的命令列表
    QString cmdStr;                                 // 发送的命令

    //lightcontroll* controller = nullptr;
    Controller* controller = nullptr;

    if(!ipPortIsOK(TermIp)){
        qDebug() << ("**************** ip地址不合法,格式范例：192.168.1.186:8886");
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ip地址不合法,格式范例：192.168.1.186:8886";
        return backJson;

    }

    //获取控制器
    // lightcontroll* controller = getControllerFromIp(TermIp);
    controller = getControllerFromIpPort(ip, port);

    if(!controllerIsUseful(controller, TermIp, backJson)){
        return backJson;
    }

    if(Broadcast != 1 && Broadcast != 0){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Broadcast 值错误 1 | 0";
        return backJson;
    }

    if(Version != 1 && Version != 2){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Version有误，只支持1/2";
        return backJson;
    }

    // content
    foreach(QJsonValue value, Content){     // 获取实际发送内容的字符串
        ContentStr += value.toString();
    }

    // 需要更改内容
    if(Broadcast){              // 广播模式
        cmdStr = QString(DISPLAY_FONT).replace("%1", "FF");     // 需要发送的命令
        if(contentSize == 0){           // 关灯

            // 关闭电源
            emit controller->signalLightPowerOn(false);

            cmdStr.replace("%2", "A0 F0");
        }else if(contentSize == 1){     // 开灯

            // 打开电源
            emit controller->signalLightPowerOn(true);

            cmdStr.replace("%2", qstr2Hex(Content.at(0).toString()));
        }else{
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "广播模式下 content 数量只能为 0 或者 1";
            return backJson;
        }
        sendDataList.insert(0, cmdStr.replace("%1","FF").toUpper());
    }else{                      // 多文字模式

        if(TermIdSize != contentSize){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "多文字模式下 TermId 数量只能跟 content 数量相等";
            return backJson;
        }

        // 打开电源
        emit controller->signalLightPowerOn(true);

        for(int i=0; i<TermIdSize; i++){
            cmdStr = DISPLAY_FONT;
            int Id = TermId.at(i).toInt();
            QString word = Content.at(i).toString().replace(" ","").trimmed();

            sendDataList.append(QString(cmdStr)
                                    .arg(Id, 2, 16, QLatin1Char('0'))
                                    .arg(qstr2Hex(word)).toUpper());
        }
    }


    //需要更改亮度
    if(json.find("Luminance") != json.end()){
        if(!json["Luminance"].isDouble()){              // 亮度类型不合法
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Luminance 类型不为int";
            return backJson;
        }
        cmdStr = CMD_LUMINANCE;
        Luminance = json["Luminance"].toInt();          // 文字亮度
        if(Luminance < 1 || Luminance > 100) {          // 亮度值不合法
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "亮度值不合法";
            return backJson;
        }
        sendDataList.insert(0, QString(cmdStr).arg(Luminance, 2, 16, QLatin1Char('0')).toUpper());
    }else{
        backJson.insert("Luminance", "亮度不存在，不做修改");
    }

    //需要更改闪烁
    if(json.find("Flicker") != json.end()){
        cmdStr = CMD_FLICKER;
        Flicker = json["Flicker"].toArray();
        int flickerSize = Flicker.size();
        int sec1 = 0, sec2 = 0;
        //qDebug() << "flickerSize = " << flickerSize;
        if(flickerSize == 0){    //关闭闪烁
            FlickerList = "0,0";
            sendDataList.insert(0, cmdStr.arg(16,2,16,QLatin1Char('0'))
                                       .arg(0,2,16,QLatin1Char('0')).toUpper());
            //emit signalUpdateFF88(sendDataList.at(0));
        } else if(flickerSize == 2){        //如果参数数量正确
            sec1 = Flicker.at(0).toInt();
            sec2 = Flicker.at(1).toInt();

            if(sec1 == 0 || sec2 == 0){     // 关闭闪烁
                FlickerList = "0,0";
                sendDataList.insert(0, cmdStr.arg(16,2,16,QLatin1Char('0'))
                                           .arg(0,2,16,QLatin1Char('0')).toUpper());
            }else if(Version == 1){         // v1 闪烁
                if(sec1 < 1000 || sec1 > 10000 || sec2 < 1000 || sec2 > 10000){
                    backJson.find("code").value() = 1;
                    backJson.find("msg").value() = "Version1 闪烁值不合法 1000~10000";
                    return backJson;
                }
                FlickerList = QString("%1,%2").arg(sec1).arg(sec2);
                sendDataList.insert(0, QString("ff 77 ff %1 %2 aa")
                                           .arg(QString::asprintf("%.0f", sec1*0.45).toInt()/100,2,16,QLatin1Char('0'))
                                           .arg(QString::asprintf("%.0f", sec2*1.18).toInt()/100,2,16,QLatin1Char('0')));
            }else if(Version == 2){         // v2 闪烁
                if(sec1 < 250 || sec1 > 10000 || sec2 < 250 || sec2 > 10000){
                    backJson.find("code").value() = 1;
                    backJson.find("msg").value() = "Version2 闪烁值不合法 250~10000";
                    return backJson;
                }
                FlickerList = QString("%1,%2").arg(sec1).arg(sec2);
                sendDataList.insert(0, cmdStr.arg(sec1/20,2,16,QLatin1Char('0'))
                                           .arg(sec2/20,2,16,QLatin1Char('0')).toUpper());
            }
            //emit signalUpdateFF88(sendDataList.at(0));
        }else {
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Flicker 闪烁值数量不对 只接受0/2个数值";
            return backJson;
        }
    }else{
        backJson.insert("Flicker", "闪烁不存在，不做修改");
    }

    if(Version == 2){
        //需要更改颜色
        if(json.find("FontColor") != json.end()){
            if(!json["FontColor"].isDouble()){              // 亮度类型不合法
                backJson.find("code").value() = 1;
                backJson.find("msg").value() = "FontColor 类型不为int";
                return backJson;
            }

            cmdStr = CHANGE_LIGHT_COLOR;        // "FF 40 FF %1 02 03 04 %5 AA"  SET_LIGHT_DEFAULT_EFFECT CHANGE_LIGHT_COLOR
            FontColor = json["FontColor"].toInt();

            if(FontColor < 1 || FontColor > 9) {          // 亮度值不合法
                backJson.find("code").value() = 1;
                backJson.find("msg").value() = "FontColor值不合法";
                return backJson;
            }

            cmdStr = QString(cmdStr).arg(FontColor,2,16,QLatin1Char('0'));
            cmdStr.replace("%2", "00");

            sendDataList.insert(0, cmdStr.toUpper());
        }else{
            backJson.insert("FontColor", "颜色不存在，不做修改");
        }
    }
    //emit controller->sigSendDatagram(sendDataList, DeviceId, name, version, fontColor, Luminance, FlickerList, TermIdSize);

    // if(Broadcast == 1){
    // }else if(Broadcast == 0){
    //     emit controller->signalBroadcastNot(sendDataList);
    // }

    //emit controller->signalSendControlCmd(sendDataList);
    QJsonObject sendDataListJson;
    sendDataListJson.insert("Broadcast", Broadcast);
    sendDataListJson.insert("ControllerIpPort", TermIp);
    sendDataListJson.insert("Content", ContentStr);
    sendDataListJson.insert("DeviceId", DeviceId);
    sendDataListJson.insert("Luminance", Luminance);
    if(Flicker.size() == 2 || json.find("Flicker") != json.end()){
        sendDataListJson.insert("Flicker", Flicker);
    }
    sendDataListJson.insert("Version", Version);
    if(Version == 2){
        sendDataListJson.insert("FontColor", FontColor);
    }

    controller->sendControlCmd(sendDataList, sendDataListJson);

    return backJson;
}

bool MyHttpServer::controllerIsUseful(Controller *controller, QString TermIp, QJsonObject &backJson)
{
    // 判断该 ip 控制器是否在线
    if(!controller){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "控制器[" + TermIp + "]不在线";
        return false;
    }

    // // 判断控制器是否处在 查询状态
    // if(controller->getCheckLightState(progress)){
    //     backJson.insert("code", 1);
    //     backJson.insert("msg", QString("正在查询雾灯状态，<%1>").arg(progress));
    //     return false;
    // }

    return true;
}

Controller *MyHttpServer::getControllerFromIpPort(QString ip, int Port)
{
    Controller* controller = nullptr;

    if(!m_controllList){
        return controller;
    }

    for(int i=0; i<m_controllList->size(); i++){
        controller = m_controllList->at(i);
        if(controller->getControllIp() == ip && controller->getControllPort() == Port){
            break;
        }else{
            controller = nullptr;
        }
    }

    return controller;
}

QJsonObject MyHttpServer::parseUpdateLightState(QJsonObject &json)
{
    QJsonObject backJson;
    backJson.insert("code", 0);
    backJson.insert("msg", "成功");

    QJsonArray ControllerList = json.value("Controllers").toArray();

    // qDebug() << "ControllerList: " << ControllerList;

    if(json.find("Controllers") == json.end()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "key 值 Controllers 不存在 ";
        return backJson;
    }else if(!json.value("Controllers").isArray()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Controllers 内数据不是列表 ";
        return backJson;
    }

    foreach(QJsonValue controllerValue, ControllerList){
        QJsonObject controllerJson = controllerValue.toObject();
        QString ControllerIpPort = controllerJson.value("ControllerIpPort").toString();
        QJsonArray LightIdsArray = controllerJson.value("LightIds").toArray();
        int CheckMode = controllerJson.value("CheckMode").toInt();
        QStringList idList;
        idList << "254";

        if(!controllerValue.isObject()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Controllers 内数据列表，内部不为json ";
            return backJson;
        }else if(controllerJson.find("CheckMode") == json.end()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "key 值 CheckMode 不存在 ";
            return backJson;
        }else if(!controllerJson.value("CheckMode").isDouble()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "CheckMode 值应该是 int ";
            return backJson;
        }

        if(controllerJson.find("ControllerIpPort") == controllerJson.end()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Controller 属性缺少 ip:port ";
            return backJson;
        }else if(controllerJson.find("LightIds") == controllerJson.end()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Controller 属性缺少 LightIds ";
            return backJson;
        }else if(!controllerJson.value("LightIds").isArray()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "Controller 的属性 LightIds 内值应该为 int 列表 ";
            return backJson;
        }

        foreach(QJsonValue value, controllerJson.value("LightIds").toArray()){
            if(!value.isDouble()){
                backJson.find("code").value() = 1;
                backJson.find("msg").value() = "Controller 的属性 LightIds 内值应该为 int 列表 ";
                return backJson;
            }
            idList << QString::number(value.toInt());
        }

        Controller* controller = nullptr;
        QString ip;
        int port;

        if( ControllerIpPort.split(":").size() != 2){
            qDebug() << ("**************** ip地址不合法  ip:port");
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "ip地址不合法  ip:port";
            return backJson;

        }

        ip = ControllerIpPort.split(":").at(0);
        port = ControllerIpPort.split(":").at(1).toInt();
        controller = getControllerFromIpPort(ip, port);             // 控制器

        if(!controllerIsUseful(controller, ControllerIpPort, backJson)){
            return backJson;
        }

        controller->sendCheckCmd(CheckMode, idList);
        // 开始工作
    }



    // Controller* controller = nullptr;                // 控制器
    // QStringList cmdList;

    // controller =

    // if(!controllerIsUseful(controller, TermIp, backJson)){
    //     return backJson;
    // }
    // emit controller->signalSendCheckCmd(cmdList);

    //emit controller->signalCheckLightState(idList);

    //qDebug() << idArrayList;




    return backJson;
}

QJsonObject MyHttpServer::parseLightPathTracking(QJsonObject &json)
{
    QJsonObject backJson;
    backJson.insert("code", 0);
    backJson.insert("msg", "成功");

    //如果必要参数不存在，或者不合理，直接返回
    if(json.find("ControllerIpPort") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 ControllerIpPort";
        return backJson;
    }else if(!json.find("ControllerIpPort")->isString()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ControllerIpPort 数据类型错误 应该为 string";
        return backJson;
    }else if(!ipPortIsOK(json.value("ControllerIpPort").toString())){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ip地址不合法,格式范例：192.168.1.186:8886";
        return backJson;
    }

    if( json.find("PathTracking") == json.end()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 PathTracking";
        return backJson;
    }else if(!json.find("PathTracking")->isDouble()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "ControllerIpPort 数据类型错误 应该为 int";
        return backJson;
    }

    // 获取数据
    QString ControllerIpPort = json["ControllerIpPort"].toString(); // 获取控制器 ip:port
    int PathTracking = json["PathTracking"].toInt();                // 获取 Broadcast
    QString ip = ControllerIpPort.split(":").at(0);                 // 获取控制器 ip
    int port = ControllerIpPort.split(":").at(1).toInt();           // 获取控制器 port

    if(PathTracking != 0 && PathTracking != 1 && PathTracking != 2 ){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "PathTracking 值只能为 0 - 2 整数";
        return backJson;
    }

    Controller* controller = nullptr;
    controller = getControllerFromIpPort(ip, port);

    // if(!controllerIsUseful(controller, ControllerIpPort, backJson)){
    //     return backJson;
    // }

    // if(!controller){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "控制器[" + ControllerIpPort + "]不在线";
    //     return backJson;
    // }

    //emit controller->signalOpenPathTracking(PathTracking);


    QJsonObject jsonObj;
    QStringList cmdList;

    switch(PathTracking){
    case 0:     // 0 - 关闭
        cmdList << QString(CMD_PATH_TRACKING_OFF).arg("FF");
        break;
    case 1:     // 1 - 模式1
        cmdList << QString(CMD_PATH_TRACKING_ON_1).arg("FF");
        break;
    case 2:     // 2 - 模式2
        cmdList << QString(CMD_PATH_TRACKING_ON_2).arg("FF");
        break;
    default:
        break;
    }

    jsonObj.insert("PathTracking", PathTracking);

    emit controller->sendControlCmd(cmdList, jsonObj);

    return backJson;
}
