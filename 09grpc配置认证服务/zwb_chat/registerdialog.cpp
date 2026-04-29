#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

registerDialog::registerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    //设置输入密码的样式为不可见
    ui->password_edit->setEchoMode(QLineEdit::Password);
    ui->password_sure_edit->setEchoMode(QLineEdit::Password);
    ui->error->setProperty("state", "normal");
    repolish(ui->error);
    connect(httpmgr::GetInstance().get(), &httpmgr::sig_http_finish,
this, &registerDialog::slot_reg_mod_finish);
    initHttpHandlers();
}

registerDialog::~registerDialog()
{
    delete ui;
}



void registerDialog::on_getcode_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        QJsonObject json_obj;
        json_obj["email"] = email;
        httpmgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),
                                            json_obj, ReqId::ID_GET_VARIFY_CODE,Modules::REGISTEMOD);
    }else{
        showTip(tr("邮箱地址不正确...."), false);
    }
}

void registerDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"), false);
        return;
    }

    //解析字符串
    QJsonDocument json = QJsonDocument::fromJson(res.toUtf8());
    if(json.isNull()){
        showTip(tr("json解析失败"), false);
        return;
    }
    if(!json.isObject()){
        showTip(tr("json解析失败"), false);
        return;
    }
    _hanlders[id](json.object());
    return;
}

void registerDialog::initHttpHandlers()
{
    _hanlders.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"), false);
            return ;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已经发送到邮箱，注意查收"), true);
        qDebug() <<"email is " << email << Qt::endl;
        return;
    });
}

void registerDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
        ui->error->setProperty("state", "normal");
    }else{
        ui->error->setProperty("state", "err");
    }
    ui->error->setText(str);


    repolish(ui->error);
}

