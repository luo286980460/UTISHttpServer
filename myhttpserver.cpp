#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>
#include <QFile>
#include <QCoreApplication>
#include <QDir>

#include "myhttpserver.h"
#include "controller.h"
#include "include/libhv/hthread.h"    // import hv_gettid
#include "include/libhv/hasync.h"     // import hv::async
#include "lightCmdList.h"
#include "qaesencryption.h"

#define LOGIN_PATH "/src/dist/index.html"
#define SRC_PATH "/src/dist"
#define ACCOUNT_INFO_PATH "/aip.ls"


MyHttpServer::MyHttpServer(int port, QObject *parent)
    : QObject(parent)
{
    createHttpserver(port);
}

MyHttpServer::~MyHttpServer()
{
    m_httpServer->stop();
    hv::async::cleanup();
}

void MyHttpServer::updateControllList(QList<Controller *> * controllList)
{
    m_controllList = controllList;
}

void MyHttpServer::setCfgJson(QJsonObject &cfgJson)
{
    m_cfgJson = cfgJson;
}

void MyHttpServer::stop()
{
    m_httpServer->stop();
    hv::async::cleanup();
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


    // 首页 login.html  /UtisWebCfgServer
    m_router->GET("/", [](HttpRequest* req, HttpResponse* resp) {

        QString path = QCoreApplication::applicationDirPath() + LOGIN_PATH;

        return resp->File(path.toStdString().c_str());
    });

    // login
    m_router->POST("/login", [this](HttpRequest* req, HttpResponse* resp) {

        // const char* Token;  = "session_id=Y3VybF91c2VyOjEyMw==; Max-Age=7200; Path=/; Domain=.example.com; Secure; HttpOnly; SameSite=Lax";
        //  resp->SetHeader("Set-Cookie", Token);

        QJsonObject backJson;
        backJson.insert("token", "");
        backJson.insert("code", 200);
        backJson.insert("msg", "ok");

        QString body = QString::fromStdString(req->body);
        QString contentType = QString::fromStdString(req->GetHeader("ConTent-Type")).trimmed().replace(" ", "");

        if("application/json" != contentType){
            backJson["code"] = 400;
            backJson["msg"] = "非法请求头";


            resp->content_type = APPLICATION_JSON;
            resp->body = QJsonDocument(backJson).toJson().toStdString();
            // resp->body = aes128_ECB_PKCS7_HEX(QJsonDocument(backJson).toJson().data(), m_aesKey).toStdString();
            return 200;

            //return resp->String(QJsonDocument(backJson).toJson().toStdString());
        }

        // 获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QString username = jsonObj.value("username").toString();
        QString password =  jsonObj.value("password").toString();
        QString token;


        //  使用数据库保存账号密码的模块
        // 验证账号密码
        // if(!m_myDataBase->useramePwdIsOK(username, password, backJson)){
        //     return resp->String(QJsonDocument(backJson).toJson().toStdString());
        // }

        // // 生成token并更新
        // token = generateToken(username, password);
        // if(!m_myDataBase->updataToken(username, token, backJson)){
        //     resp->content_type = TEXT_PLAIN;
        //     resp->body = QJsonDocument(backJson).toJson().toStdString();
        //     return 400;

        //     //return resp->String(QJsonDocument(backJson).toJson().toStdString());
        // }
        // backJson["token"] = token;

        // // 获取设备数据
        // QJsonArray deviceArray;
        // if(!m_myDataBase->getDeviceData(username, deviceArray, backJson)){

        //     resp->content_type = TEXT_PLAIN;
        //     resp->body = QJsonDocument(backJson).toJson().toStdString();
        //     return 400;
        //     // return resp->String(QJsonDocument(backJson).toJson().toStdString());
        // }

        // backJson.insert("devices", deviceArray);
        // m_myDataBase->closeDataBase(); // 关闭连接

        // return resp->String(QJsonDocument(backJson).toJson().toStdString());



        // 使用文件保存账号密码的模块
        QString accountInfoFilePath = QCoreApplication::applicationDirPath() + ACCOUNT_INFO_PATH;
        QFile accountInfoFile(accountInfoFilePath);

        if(!accountInfoFile.open(QIODevice::ReadOnly)){
            backJson["code"] = 400;
            backJson["msg"] = "account cfg missing";

            resp->content_type = APPLICATION_JSON;
            resp->body = QJsonDocument(backJson).toJson().toStdString();
            //resp->body = aes128_ECB_PKCS7_HEX(QJsonDocument(backJson).toJson().data(), m_aesKey).toStdString();
            return 200;
        }

        bool isOk = false;

        //qDebug() << QJsonDocument::fromJson(decrypt_Aes128_ECB_PKCS7_HEX(QByteArray::fromHex(accountInfoFile.readAll()), m_aesKey).trimmed());

        QByteArray res = decrypt_Aes128_ECB_PKCS7_HEX(QByteArray::fromHex(accountInfoFile.readAll()), m_aesKey);
        res.chop(res.at(res.size() - 1));

        QJsonObject usrInfo = QJsonDocument::fromJson(res).object();


        if(usrInfo.find(username) == usrInfo.end()){
            backJson["code"] = 400;
            backJson["msg"] = "用户错误";

            resp->content_type = APPLICATION_JSON;
            resp->body = QJsonDocument(backJson).toJson().toStdString();
            return 200;
        }else if(usrInfo.value(username).toString() != password){
            backJson["code"] = 400;
            backJson["msg"] = "密码错误";

            resp->content_type = APPLICATION_JSON;
            resp->body = QJsonDocument(backJson).toJson().toStdString();
            return 200;
        }

        // QString lineData = accountInfoFile.readLine();

        // while(!lineData.isEmpty()){
        //     QList usrPwdList = lineData.split(":");
        //     if(usrPwdList.size() != 2){
        //         lineData = accountInfoFile.readLine();
        //         continue;
        //     }
        //     QString usr = usrPwdList.at(0).trimmed();
        //     QString pwd = usrPwdList.at(1).trimmed();

        //     if(username == usr && password == pwd){
        //         isOk = true;
        //         break;
        //     }

        //     lineData = accountInfoFile.readLine();
        // }

        // if(!isOk){
        //     backJson["code"] = 400;
        //     backJson["msg"] = "用户或者密码错误";

        //     resp->content_type = APPLICATION_JSON;
        //     resp->body = QJsonDocument(backJson).toJson().toStdString();
        //     //resp->body = aes128_ECB_PKCS7_HEX(QJsonDocument(backJson).toJson().data(), m_aesKey).toStdString();
        //     return 200;
        // }

        backJson["code"] = 200;
        backJson["msg"] = "success";
        backJson["token"] = "1";
        resp->content_type = APPLICATION_JSON;
        resp->body = QString(QJsonDocument(backJson).toJson()).toStdString();
        // resp->body = aes128_ECB_PKCS7_HEX(QJsonDocument(backJson).toJson().data(), m_aesKey).toStdString();
        return 200;

    });

    // 警示灯
    // 广播控灯
    m_router->POST("/light/Broadcast", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();


        qDebug() << jsonDoc;

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }



        return respReturnJson(resp, parseLightBroadcast(jsonObj));
    });

    m_router->POST("/jingShiDeng/Broadcast", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        qDebug() << jsonDoc;

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightBroadcast(jsonObj));
    });

    // 非广播控灯
    m_router->POST("/light/BroadcastNot", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightBroadcastNot(jsonObj));
    });

    m_router->POST("/jingShiDeng/BroadcastNot", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightBroadcastNot(jsonObj));
    });

    // 雾灯轨迹开关
    m_router->POST("/light/PathTracking", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightPathTracking(jsonObj));
    });

    m_router->POST("/jingShiDeng/PathTracking", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightPathTracking(jsonObj));
    });

    // 雾灯状态更新
    m_router->POST("/light/UpdateLightState", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseUpdateLightState(jsonObj));
    });

    m_router->POST("/jingShiDeng/UpdateLightState", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();


        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseUpdateLightState(jsonObj));
    });

    // 重启警示灯服务
    m_router->GET("/jingShiDeng/restart", [this](HttpRequest* req, HttpResponse* resp) {
        Q_UNUSED(req);


        QJsonObject backJson;
        backJson.insert("code", 200);
        backJson.insert("msg", "success");

        emit signalRestartApplication();


        return respReturnJson(resp, parseUpdateLightState(backJson));
    });

    // 拉取警示灯配置信息
    m_router->GET("/jingShiDeng/getCfgInfo", [this](HttpRequest* req, HttpResponse* resp) {

        Q_UNUSED(req);
        // QJsonObject cfgJson;
        // QJsonArray lights;
        // QJsonObject light1;
        // QJsonObject light2;

        // cfgJson.insert("serverPort", "2333");
        // cfgJson.insert("sendingInterval", "9999");
        // cfgJson.insert("sendingCount", "10");
        // cfgJson.insert("topic", "deviceState");
        // cfgJson.insert("kafkaIp", "58.240.67.138");
        // cfgJson.insert("kafkaPort", "18543");
        // cfgJson.insert("kafkaTopics", "deviceState,nh_deviceState");

        // light1.insert("name", "控制器1");
        // light1.insert("ipPort", "192.168.1.101:8886");
        // light1.insert("lightId", "1-4");
        // light1.insert("quanXiId", "QuanXiId1, QuanXiId2, QuanXiId3, QuanXiId4");
        // light1.insert("connectType", "tcp");



        // light2.insert("name", "控制器2");
        // light2.insert("ipPort", "192.168.1.101:8886");
        // light2.insert("lightId", "2-8");
        // light2.insert("quanXiId", "QuanXiId1, QuanXiId2, QuanXiId3, QuanXiId4");
        // light2.insert("connectType", "udp");


        // lights << light1 << light2;
        // cfgJson.insert("controllers", lights);

        resp->content_type = APPLICATION_JSON;
        // resp->body = QString(QJsonDocument(m_cfgJson).toJson()).toUtf8().toStdString();

        resp->body = crypt_Aes128_ECB_PKCS7_HEX(QJsonDocument(m_cfgJson).toJson(), m_aesKey).toStdString();
        return 200;

    });

    // 提交警示灯配置信息
    m_router->POST("/jingShiDeng/setCfgInfo", [this](HttpRequest* req, HttpResponse* resp) {

        // const char* Token;  = "session_id=Y3VybF91c2VyOjEyMw==; Max-Age=7200; Path=/; Domain=.example.com; Secure; HttpOnly; SameSite=Lax";
        //  resp->SetHeader("Set-Cookie", Token);


        QJsonObject backJson;
        backJson.insert("code", 200);
        backJson.insert("msg", "ok");


        // 控制器是否有重复

        // if(){
        //     return
        // }

        // QString body = QString::fromStdString(req->body);
        // //QString contentType = QString::fromStdString(req->GetHeader("ConTent-Type")).trimmed().replace(" ", "");
        // qDebug() << body;


        // // QString body = QString::fromStdString(req->body);
        // // QString contentType = QString::fromStdString(req->GetHeader("ConTent-Type")).trimmed().replace(" ", "");
        // // if("application/json" != contentType){
        // //     backJson["code"] = 400;
        // //     backJson["msg"] = "非法请求头";


        // //     resp->content_type = APPLICATION_JSON;
        // //     resp->body = QJsonDocument(backJson).toJson().toStdString();
        // //     return 200;
        // // }

        qDebug() << "提交警示灯配置信息: " << req->body.c_str();

        emit signalSetCfgJson(req->body.c_str());

        resp->content_type = APPLICATION_JSON;
        resp->body = QString(QJsonDocument(backJson).toJson()).toUtf8().toStdString();
        //resp->body = req->Body();
        return 200;
    });

    // 旧版控灯协议 2.0
    m_router->POST("/light", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();
        qDebug() << jsonDoc;

        //qDebug()<< jsonObj;

        return resp->String(QJsonDocument(parseLightJson(jsonObj)).toJson().toStdString());
    });

    // 警示灯跑马灯开关
    m_router->POST("/jingShiDeng/Marquee", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightMarquee(jsonObj, backJson));
    });

    // 警示灯红蓝警示灯开关
    m_router->POST("/jingShiDeng/RedAndBlue", [this](HttpRequest* req, HttpResponse* resp) {

        //获取json数据包
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(req->body).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();


        QJsonObject backJson;
        backJson["code"] = 200;
        backJson["msg"] = "success";

        if(!headerIsOk(req, backJson)){
            return respReturnJson(resp, backJson);
        }

        return respReturnJson(resp, parseLightRedAndBlue(jsonObj, backJson));
    });

    // ******************       安全桩开始       ************************

    // 拉取安全桩配置信息
    m_router->GET("/anQuanZhuang/getCfgInfo", [this](HttpRequest* req, HttpResponse* resp) {

        Q_UNUSED(req);
        QJsonObject anQuanZhuangCfgJson;

        anQuanZhuangCfgJson.insert("LEDIP","192.168.0.55:8000");
        anQuanZhuangCfgJson.insert("anQuanZhuangIP","192.168.0.55:8000");
        anQuanZhuangCfgJson.insert("screenContentBefore","安全行车");
        anQuanZhuangCfgJson.insert("screenContentAfter","文明驾驶");
        anQuanZhuangCfgJson.insert("fontColor","红");
        anQuanZhuangCfgJson.insert("trafficLightColor","红");



        resp->content_type = APPLICATION_JSON;
        // resp->body = QString(QJsonDocument(m_cfgJson).toJson()).toUtf8().toStdString();

        resp->body = crypt_Aes128_ECB_PKCS7_HEX(QJsonDocument(anQuanZhuangCfgJson).toJson(), m_aesKey).toStdString();
        return 200;

    });

    // 提交安全桩配置信息
    m_router->POST("/anQuanZhuang/setCfgInfo", [](HttpRequest* req, HttpResponse* resp) {

        // const char* Token;  = "session_id=Y3VybF91c2VyOjEyMw==; Max-Age=7200; Path=/; Domain=.example.com; Secure; HttpOnly; SameSite=Lax";
        //  resp->SetHeader("Set-Cookie", Token);


        QJsonObject backJson;
        backJson.insert("code", 200);
        backJson.insert("msg", "ok");


        // 控制器是否有重复

        // if(){
        //     return
        // }

        // QString body = QString::fromStdString(req->body);
        // //QString contentType = QString::fromStdString(req->GetHeader("ConTent-Type")).trimmed().replace(" ", "");
        // qDebug() << body;


        // // QString body = QString::fromStdString(req->body);
        // // QString contentType = QString::fromStdString(req->GetHeader("ConTent-Type")).trimmed().replace(" ", "");
        // // if("application/json" != contentType){
        // //     backJson["code"] = 400;
        // //     backJson["msg"] = "非法请求头";


        // //     resp->content_type = APPLICATION_JSON;
        // //     resp->body = QJsonDocument(backJson).toJson().toStdString();
        // //     return 200;
        // // }

        qDebug() << "提安全桩配置信息: " << req->body.c_str();

        // emit signalSetCfgJson(req->body.c_str());

        resp->content_type = APPLICATION_JSON;
        resp->body = QString(QJsonDocument(backJson).toJson()).toUtf8().toStdString();
        //resp->body = req->Body();
        return 200;
    });

    // 重启安全桩服务
    m_router->GET("/anQuanZhuang/restart", [](HttpRequest* req, HttpResponse* resp) {
        Q_UNUSED(req);


        QJsonObject backJson;
        backJson.insert("code", 200);
        backJson.insert("msg", "ok");



        resp->content_type = APPLICATION_JSON;
        resp->body = QJsonDocument(backJson).toJson().toStdString();
        return 200;
    });

     // ******************       安全桩结束       ************************


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

        QJsonObject backJson;
        backJson.insert("time", "最后更新时间：2025年06月20日");
        backJson.insert("Name", "尤特斯警示灯服务");
        backJson.insert("Version", "1.0.0");
        backJson.insert("Msg", "红蓝功能已加, tcp控灯已修复, 跑马功能本地已测试,大批量灯未测试,没有环境");

        resp->content_type = APPLICATION_JSON;
        resp->body = QJsonDocument(backJson).toJson().toStdString();


        // resp->body = aes128_ECB_PKCS7_HEX(QJsonDocument(backJson).toJson(), m_aesKey).toStdString();
        return 200;
        // return resp->Json(ex3);
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

    // web 模块


    m_httpServer = new HttpServer;


    add_directory_handlers(*m_httpServer, QCoreApplication::applicationDirPath() + SRC_PATH);

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

    // if(json.find("name") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 name ";
    //     return true;
    // }else if(!json.find("name")->isString()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "name 数据类型错误 应该为 string";
    //     return true;
    // }

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

    // name
    // if(json.find("Name") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 Name ";
    //     return true;
    // }else if(!json.value("name").isString()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Name 数据类型错误 应该为 string";
    //     return true;
    // }

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
    backJson.insert("code", 200);
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

    if(!controller) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "控制器：" + ControllerIpPort + "不在线";
        return backJson;
    }

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

            if(content == "a"){
                cmdStr.replace("%2", "A0 D5");
            }else{
                cmdStr.replace("%2", qstr2Hex(content));
            }
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


    // 需要更改轨迹模式
    if( json.find("PathTracking") != json.end()){
        if(!json.find("PathTracking")->isDouble()){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "PathTracking 数据类型错误 应该为 int";
            return backJson;
        }

        int PathTracking = json.value("PathTracking").toInt();
        if(PathTracking != 0 && PathTracking != 1 && PathTracking != 2 ){
            backJson.find("code").value() = 1;
            backJson.find("msg").value() = "PathTracking 值只能为 0 - 2 整数";
            return backJson;
        }

        // QJsonObject jsonObj;
        // QStringList cmdList;

        switch(PathTracking){
        case 0:     // 0 - 关闭
            sendDataList.insert(0, QString(CMD_PATH_TRACKING_OFF).arg("FF"));
            break;
        case 1:     // 1 - 模式1
            sendDataList.insert(0, QString(CMD_PATH_TRACKING_ON_1).arg("FF"));
            break;
        case 2:     // 2 - 模式2
            sendDataList.insert(0, QString(CMD_PATH_TRACKING_ON_2).arg("FF"));
            break;
        default:
            break;
        }
    }

    if(!controllerIsUseful(controller, ControllerIpPort, backJson)){
        return backJson;
    }

    // 发送命令
    controller->sendControlCmdBroadcast(sendDataList, json);

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

    // name
    // if(json.find("Name") == json.end()) {
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "缺少必要参数 Name ";
    //     return true;
    // }else if(!json.value("name").isString()){
    //     backJson.find("code").value() = 1;
    //     backJson.find("msg").value() = "Name 数据类型错误 应该为 string";
    //     return true;
    // }

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
    backJson.insert("code", 200);
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
        QString content = lightJson.value("Content").toString();

        if(content == "a"){
            sendDataList.append(QString(cmdStr)
                                    .arg(lightJson.value("LightId").toInt(), 2, 16, QLatin1Char('0'))
                                    .arg("A0 D5").toUpper());
        }else{
            sendDataList.append(QString(cmdStr)
                                    .arg(lightJson.value("LightId").toInt(), 2, 16, QLatin1Char('0'))
                                    .arg(qstr2Hex(lightJson.value("Content").toString())).toUpper());
        }
    }

    // 打开电源
    emit controller->signalLightPowerOn(true);
    // controller->signalSendControlCmd(sendDataList);
    controller->sendControlCmdBroadcastNot(sendDataList, json);

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
    backJson.insert("code", 200);
    backJson.insert("msg", "成功");

    //如果必要参数不存在，或者不合理，直接返回
    if(missingParameter(json, backJson)){
        return backJson;
    }

    QString TermIp = json["TermIp"].toString();     // 获取控制器 ip:port
    QString ip = TermIp.split(":").at(0);           // 获取控制器 ip
    int port = TermIp.split(":").at(1).toInt();     // 获取控制器 port
    QString Name = json["name"].toString();         // 获取 Name

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
    sendDataListJson.insert("Name", Name);
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

void MyHttpServer::add_file_handler(HttpServer &server, const QString &basepath, const QString &path)
{
    QString filepath = basepath + "/" + path;
    QFileInfo fileInfo(filepath);
    if (fileInfo.isFile()) {
        m_router->GET(("/"+path).toStdString().c_str(), [filepath](HttpRequest* req, HttpResponse* resp) {

            return resp->File(filepath.toStdString().c_str());
        });
    }
}

void MyHttpServer::add_directory_handlers(HttpServer &server, const QString &basepath, const QString &path)
{
    QDir dir(basepath + "/" + path);
    if (dir.exists()) {
        for (const QString& entry : dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
            QString subpath = path.isEmpty() ? entry : path + "/" + entry;
            if (QFileInfo(dir, entry).isDir()) {
                add_directory_handlers(server, basepath, subpath);
            } else {
                add_file_handler(server, basepath, subpath);
            }
        }
    }
}

QByteArray MyHttpServer::crypt_Aes128_ECB_PKCS7_HEX(QByteArray plaintext, QByteArray key)
{
    return QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::ECB, plaintext, key, "", QAESEncryption::PKCS7).toHex();
}

QByteArray MyHttpServer::decrypt_Aes128_ECB_PKCS7_HEX(QByteArray plaintext, QByteArray key)
{
    return QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::ECB, plaintext, key, "", QAESEncryption::PKCS7);
}

bool MyHttpServer::headerIsOk(HttpRequest *req, QJsonObject &backJson)
{
    if(QString::fromStdString(req->GetHeader("Content-Type")).toLower() != "application/json"){
        backJson["code"] = 400;
        backJson["msg"] = "请求头需要 application/json";
        return false;
    }
    return true;
}

int MyHttpServer::respReturnJson(HttpResponse *resp, QJsonObject json)
{
    resp->content_type = APPLICATION_JSON;
    resp->body = QJsonDocument(json).toJson().toStdString().c_str();
    return 200;
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
    backJson.insert("code", 200);
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
    backJson.insert("code", 200);
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

QJsonObject MyHttpServer::parseLightMarquee(QJsonObject &json, QJsonObject &backJson)
{
    //如果必要参数不存在，或者不合理，直接返回
    if(missingParameterMarquee(json, backJson)){
        return backJson;
    }

    QString ControllerIpPort                                // 控制器 ip:port
        = json["ControllerIpPort"].toString();
    QString ip = ControllerIpPort.split(":").at(0);         // 控制器 ip
    int port = ControllerIpPort.split(":").at(1).toInt();   // 控制器 port
    Controller* controller = getControllerFromIpPort(ip, port); // 控制器

    if(!controller) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "控制器：" + ControllerIpPort + "不在线";
        return backJson;
    }

    controller->sendControlCmdMarquee(json);

    return backJson;
}

bool MyHttpServer::missingParameterMarquee(QJsonObject &json, QJsonObject &backJson)
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

    // 控制器 ip:port
    if(json.find("Switch") == json.end()) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "缺少必要参数 Switch ";
        return true;
    }else if(!json.value("Switch").isBool()){
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "Switch 数据类型错误 应该为 bool";
        return true;
    }

    return false;
}

QJsonObject MyHttpServer::parseLightRedAndBlue(QJsonObject &json, QJsonObject &backJson)
{
    //如果必要参数不存在，或者不合理，直接返回
    if(missingParameterMarquee(json, backJson)){
        return backJson;
    }

    QStringList sendDataList;                       // 需要发送的命令列表
    QString ControllerIpPort                                // 控制器 ip:port
        = json["ControllerIpPort"].toString();
    QString ip = ControllerIpPort.split(":").at(0);         // 控制器 ip
    int port = ControllerIpPort.split(":").at(1).toInt();   // 控制器 port
    Controller* controller = getControllerFromIpPort(ip, port); // 控制器

    if(!controller) {
        backJson.find("code").value() = 1;
        backJson.find("msg").value() = "控制器：" + ControllerIpPort + "不在线";
        return backJson;
    }

    return backJson;
}
