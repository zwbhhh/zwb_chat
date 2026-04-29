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
#include <QTimer>
// #include <QCursor>  //鼠标事件的头文件....

extern std::function<void(QWidget*)> repolish;

extern std::function<QString(QString)> xorString;

extern QString gate_url_prefix;

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

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickLbState{
    Normal = 0,
    Selected = 1
};


#endif // GLOB A L_H
