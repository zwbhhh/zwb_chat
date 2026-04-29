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
    });

    // 注册注册用户回包逻辑
    _hanlders.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug() << "email is " << email ;
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


void registerDialog::on_sure_btn_clicked()
{
    if(ui->user_edit->text() == ""){
        showTip(tr("用户名不能为空"), false);
        return;
    }
    if(ui->email_edit->text() == ""){
        showTip(tr("邮箱不能为空"), false);
        return;
    }
    if(ui->password_edit->text() == ""){
        showTip(tr("密码不能为空"), false);
        return;
    }
    if(ui->password_sure_edit->text() == ""){
        showTip(tr("确认密码不能为空"), false);
        return;
    }
    if(ui->password_sure_edit->text() != ui->password_edit->text()){
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }
    if(ui->verifycode_edit->text() == ""){
        showTip(tr("验证码不能为空"), false);
        return;
    }
    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->password_edit->text();
    json_obj["confirm"] = ui->password_sure_edit->text();
    json_obj["varifycode"] = ui->verifycode_edit->text();
    httpmgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTEMOD);
}

