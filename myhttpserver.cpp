#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "myhttpserver.h"
#include "include/libhv/hthread.h"    // import hv_gettid
#include "include/libhv/hasync.h"     // import hv::async
//#include "lightCmdList.h"

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
        Json ex3 = {
                    {"happy", true},
                    {"pi", 11111111111111111},
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


    // 警示灯
    m_router->POST("/light", [this](HttpRequest* req, HttpResponse* resp) {
        QJsonObject backJson;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        //emit showMsg(QJsonDocument(jsonObj).toJson());

        qDebug() << " ***** " << jsonObj;

        //backJson = parseLightJson(jsonObj);

        return resp->String(QJsonDocument(backJson).toJson().toStdString());
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

// void MyHttpServer::create(QHostAddress address, int port)
// {
//     // 初始化http服务
//     m_address = address;
//     m_port = port;
//     m_httpServer = new QHttpServer(this);

//     // m_httpServer->route("/get", [](const QHttpServerRequest &request) {
//     //     // 处理GET请求
//     //     return QHttpServerResponse::ok("Hello, GET request received!");
//     // });
//     m_httpServer->route("/", QHttpServerRequest::Method::Get,
//                         [](const QHttpServerRequest &request) {
//         //emit showMsg(QJsonDocument::fromJson(request.body()).toJson());

//         //获取json数据包

//         // QJsonDocument jsonDoc = QJsonDocument::fromJson();
//         // QJsonObject json1 = jsonDoc.object();
//         // //showMsg(request.body());

//         //qDebug() << "query: " << request.query().toString();
//         //qDebug() << "body: " << json1.value("txt").toString().data();

//         // QByteArray str = msg.toLocal8Bit();
//         // QTextCodec *gbk = QTextCodec::codecForName("GB18030");
//         // QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
//         // QString g2u = gbk->toUnicode(str);
//         // QString as = g2u.toUtf8();

//         QJsonObject json;
//         json.insert("code", 200);
//         json.insert("msg", "get");

//         return json;

//     });

//     m_httpServer->route("/get", QHttpServerRequest::Method::Get,
//                          [](const QHttpServerRequest &request) {
//         //emit showMsg(QJsonDocument::fromJson(request.body()).toJson());

//         //获取json数据包
//         //QJsonDocument jsonDoc = QJsonDocument::fromJson(request.body());
//         //QJsonObject json = jsonDoc.object();
//         //showMsg(request.body());

//         qDebug() << "query: " << request.query().toString();
//         qDebug() << "body: " << request.body();


//         QJsonObject json;
//         json.insert("code", 200);
//         json.insert("msg", "get");

//         return json;

//     });

//     m_httpServer->route("/post", QHttpServerRequest::Method::Post,
//                         [](const QHttpServerRequest &request) {
//         // qDebug() << "query: " << request.query().toString();
//         // qDebug() << "body: " << request.body();




//         //qDebug() << QString(request.body()).toLocal8Bit().data();
//         qDebug() << QString::fromUtf8(request.body());

//         QJsonObject json;
//         json.insert("code", 200);
//         json.insert("msg", "post");

//         return json;

//     });


//     m_httpServer->listen(address, m_port);
//     emit showMsg(QString("http服务已开启，监听地址:%1  监听端口:%2").arg(m_address.toString()).arg(m_port));
// }

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

// QJsonObject MyHttpServer::parseLightJson(QJsonObject &json)
// {
//     QJsonObject backJson;
//     backJson.insert("code", 0);
//     backJson.insert("msg", "成功");


//     //如果必要参数不存在，或者不合理，直接返回
//     if(missingParameter(json, backJson)){
//         return backJson;
//     }

//     QString ip;                                     // 控制器 ip
//     int port;                                       // 控制器 port
//     QString TermIp = json["TermIp"].toString();     // 获取控制器 ip:port
//     QString name = json["name"].toString();         // 获取 name
//     QString DeviceId = json["DeviceId"].toString(); // 获取 DeviceId
//     int Broadcast = json["Broadcast"].toInt();      // 获取 Broadcast
//     QJsonArray TermId = json["TermId"].toArray();   // 获取雾灯编号 TermId
//     int TermIdSize = TermId.size();                 // 雾灯编号数量
//     int fontColor = -1;                              // 文字颜色
//     int Luminance = -1;                             // 亮度值
//     int version = json["Version"].toInt();          // 获取版本号
//     QJsonArray Content = json["Content"].toArray(); // 文字内容 array
//     QString ContentStr;                             // 实际发送内容的字符串
//     int contentSize = Content.size();               // 文字内容数量
//     QJsonArray Flicker;                             // 闪烁值列表
//     QString FlickerList = "no";                     // 闪烁值字符串
//     QStringList sendDataList;                       // 需要发送的命令列表
//     QString cmdStr;                                 // 发送的命令

//     //lightcontroll* controller = nullptr;

//     if( TermIp.split(":").size() != 2){
//         qDebug() << ("**************** ip地址不合法  ip:port");
//         backJson.find("code").value() = 1;
//         backJson.find("msg").value() = "ip地址不合法  ip:port";
//         return backJson;

//     }

//     ip = TermIp.split(":").at(0);
//     port = TermIp.split(":").at(1).toInt();

//     if(port < 1 || port > 65535){    // 判断ip是否合法
//         backJson.find("code").value() = 1;
//         backJson.find("msg").value() = "port不合法";
//         qDebug() << ("**************** port不合法");
//         return backJson;
//     }

//     if(!ipAddrIsOK(ip)){    // 判断ip是否合法
//         backJson.find("code").value() = 1;
//         backJson.find("msg").value() = "ip地址不合法";
//         //showMsg("**************** ip地址不合法");
//         return backJson;
//     }
//     //获取控制器
//     // lightcontroll* controller = getControllerFromIp(TermIp);
//     //controller = getControllerFromIpPort(ip, port);

//     // 判断该 ip 控制器是否在线
//     // if(!controller){
//     //     backJson.find("code").value() = 1;
//     //     backJson.find("msg").value() = "控制器[" + TermIp + "]不在线";
//     //     return backJson;
//     // }

//     if(Broadcast != 1 && Broadcast != 0){
//         backJson.find("code").value() = 1;
//         backJson.find("msg").value() = "Broadcast 值错误 1 | 0";
//         return backJson;
//     }

//     if(version != 1 && version != 2){
//         backJson.find("code").value() = 1;
//         backJson.find("msg").value() = "version有误，只支持1/2";
//         return backJson;
//     }

//     // content
//     foreach(QJsonValue value, Content){     // 获取实际发送内容的字符串
//         ContentStr += value.toString();
//     }

//     // 需要更改内容
//     if(Broadcast){              // 广播模式
//         cmdStr = QString(DISPLAY_FONT).replace("%1", "FF");     // 需要发送的命令
//         if(contentSize == 0){           // 关灯
//             cmdStr.replace("%2", "A0 F0");
//         }else if(contentSize == 1){     // 开灯
//             cmdStr.replace("%2", qstr2Hex(Content.at(0).toString()));
//         }else{
//             backJson.find("code").value() = 1;
//             backJson.find("msg").value() = "广播模式下 content 数量只能为 0 或者 1";
//             return backJson;
//         }
//         sendDataList.insert(0, cmdStr.replace("%1","FF").toUpper());
//     }else{                      // 多文字模式
//         if(TermIdSize != contentSize){
//             backJson.find("code").value() = 1;
//             backJson.find("msg").value() = "多文字模式下 TermId 数量只能跟 content 数量相等";
//             return backJson;
//         }

//         for(int i=0; i<TermIdSize; i++){
//             cmdStr = DISPLAY_FONT;
//             int Id = TermId.at(i).toInt();
//             QString word = Content.at(i).toString().replace(" ","").trimmed();

//             sendDataList.append(QString(cmdStr)
//                                     .arg(Id, 2, 16, QLatin1Char('0'))
//                                     .arg(qstr2Hex(word)).toUpper());
//         }
//     }


//     //需要更改亮度
//     if(json.find("Luminance") != json.end()){
//         if(!json["Luminance"].isDouble()){              // 亮度类型不合法
//             backJson.find("code").value() = 1;
//             backJson.find("msg").value() = "Luminance 类型不为int";
//             return backJson;
//         }
//         cmdStr = CMD_LUMINANCE;
//         Luminance = json["Luminance"].toInt();          // 文字亮度
//         if(Luminance < 1 || Luminance > 100) {          // 亮度值不合法
//             backJson.find("code").value() = 1;
//             backJson.find("msg").value() = "亮度值不合法";
//             return backJson;
//         }
//         sendDataList.insert(0, QString(cmdStr).arg(Luminance, 2, 16, QLatin1Char('0')).toUpper());
//     }else{
//         backJson.insert("Luminance", "亮度字段不存在，不做修改");
//     }

//     //需要更改闪烁
//     if(json.find("Flicker") != json.end()){
//         cmdStr = CMD_FLICKER;
//         Flicker = json["Flicker"].toArray();
//         int flickerSize = Flicker.size();
//         int sec1 = 0, sec2 = 0;
//         //qDebug() << "flickerSize = " << flickerSize;
//         if(flickerSize == 0){    //关闭闪烁
//             FlickerList = "0,0";
//             sendDataList.insert(0, cmdStr.arg(16,2,16,QLatin1Char('0'))
//                                        .arg(0,2,16,QLatin1Char('0')).toUpper());
//             //emit signalUpdateFF88(sendDataList.at(0));
//         } else if(flickerSize == 2){        //如果参数数量正确
//             sec1 = Flicker.at(0).toInt();
//             sec2 = Flicker.at(1).toInt();

//             if(sec1 == 0 || sec2 == 0){     // 关闭闪烁
//                 FlickerList = "0,0";
//                 sendDataList.insert(0, cmdStr.arg(16,2,16,QLatin1Char('0'))
//                                            .arg(0,2,16,QLatin1Char('0')).toUpper());
//             }else if(version == 1){         // v1 闪烁
//                 if(sec1 < 1000 || sec1 > 10000 || sec2 < 1000 || sec2 > 10000){
//                     backJson.find("code").value() = 1;
//                     backJson.find("msg").value() = "version1 闪烁值不合法 1000~10000";
//                     return backJson;
//                 }
//                 FlickerList = QString("%1,%2").arg(sec1).arg(sec2);
//                 sendDataList.insert(0, QString("ff 77 ff %1 %2 aa")
//                                            .arg(QString::asprintf("%.0f", sec1*0.45).toInt()/100,2,16,QLatin1Char('0'))
//                                            .arg(QString::asprintf("%.0f", sec2*1.18).toInt()/100,2,16,QLatin1Char('0')));
//             }else if(version == 2){         // v2 闪烁
//                 if(sec1 < 250 || sec1 > 10000 || sec2 < 250 || sec2 > 10000){
//                     backJson.find("code").value() = 1;
//                     backJson.find("msg").value() = "version2 闪烁值不合法 250~10000";
//                     return backJson;
//                 }
//                 FlickerList = QString("%1,%2").arg(sec1).arg(sec2);
//                 sendDataList.insert(0, cmdStr.arg(sec1/20,2,16,QLatin1Char('0'))
//                                            .arg(sec2/20,2,16,QLatin1Char('0')).toUpper());
//             }
//             //emit signalUpdateFF88(sendDataList.at(0));
//         }else {
//             backJson.find("code").value() = 1;
//             backJson.find("msg").value() = "Flicker 闪烁值数量不对 只接受0/2个数值";
//             return backJson;
//         }
//     }else{
//         backJson.insert("Flicker", "闪烁字段不存在，不做修改");
//     }

//     if(version == 2){
//         //需要更改颜色
//         if(json.find("FontColor") != json.end()){
//             if(!json["FontColor"].isDouble()){              // 亮度类型不合法
//                 backJson.find("code").value() = 1;
//                 backJson.find("msg").value() = "FontColor 类型不为int";
//                 return backJson;
//             }

//             cmdStr = CHANGE_LIGHT_COLOR;        // "FF 40 FF %1 02 03 04 %5 AA"  SET_LIGHT_DEFAULT_EFFECT CHANGE_LIGHT_COLOR
//             fontColor = json["FontColor"].toInt();

//             if(fontColor < 1 || fontColor > 9) {          // 亮度值不合法
//                 backJson.find("code").value() = 1;
//                 backJson.find("msg").value() = "FontColor值不合法";
//                 return backJson;
//             }

//             cmdStr = QString(cmdStr).arg(fontColor,2,16,QLatin1Char('0'));
//             cmdStr.replace("%2", "00");

//             sendDataList.insert(0, cmdStr.toUpper());
//         }else{
//             backJson.insert("FontColor", "颜色字段不存在，不做修改");
//         }
//     }

//     //emit controller->sigSendDatagram(sendDataList, DeviceId, name, version, fontColor, Luminance, FlickerList, TermIdSize);

//     return backJson;
// }

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
