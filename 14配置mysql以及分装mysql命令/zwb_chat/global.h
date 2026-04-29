#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QStyle>
#include <QRegularExpression>
#include <QDebug>
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>
// #include <QCursor>  //鼠标事件的头文件....



enum ReqId{
    ID_GET_VARIFY_CODE = 1001,
    ID_REG_USER = 1002
};
enum Modules{
    REGISTEMOD = 0,
};
enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1,
    ERR_NETWORK = 2,
};
extern std::function<void(QWidget*)> repolish;

extern QString gate_url_prefix;

#endif // GLOB A L_H
