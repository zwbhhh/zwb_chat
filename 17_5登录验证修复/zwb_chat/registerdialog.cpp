#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

registerDialog::registerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerDialog),_countdown(5)
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

    ui->error->clear();
    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->password_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->password_sure_edit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->verifycode_edit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->comfirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");
    ui->comfirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");

    connect(ui->pass_visible, &ClickLabel::clicked, this, [this]() {
        auto state = ui->pass_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->password_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->password_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });
    connect(ui->comfirm_visible, &ClickLabel::clicked, this, [this]() {
        auto state = ui->comfirm_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->password_sure_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->password_sure_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    // 创建定时器
    _countdown_timer = new QTimer(this);
    // 连接信号和槽
    connect(_countdown_timer, &QTimer::timeout, [this](){
        if(_countdown==0){
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回登录").arg(_countdown);
        ui->tip_lb->setText(str);
    });
}



registerDialog::~registerDialog()
{
    qDebug()<<"destruct RegisterDialog";
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
    //调用对应的逻辑,根据id回调。
    //这行代码直接调用 _handlers[id] ，但是存在一个严重的问题：
    auto it = _hanlders.find(id);
    if(it != _hanlders.end()){
        (*it)(json.object());
    }else{
        showTip(tr("未知的请求ID"), false);
        qDebug() << "Unknown request ID: " << id;
    }
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
        qDebug() << "user uuid is " <<jsonObj["uid"].toString();
        qDebug() << "email is " << email ;
        ChangeTipPage();
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

void registerDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}
void registerDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->error->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

void registerDialog::ChangeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    //启动定时器,设置时间间隔为1000毫秒(1秒)
    _countdown_timer->start(1000);
}

bool registerDialog::checkUserValid()
{
    if(ui->user_edit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool registerDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool registerDialog::checkPassValid()
{
    auto pass = ui->password_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool registerDialog::checkVarifyValid()
{
    auto pass = ui->verifycode_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

bool registerDialog::checkConfirmValid()
{
    auto pass = ui->password_edit->text();
    auto confirm = ui->password_sure_edit->text();

    if(confirm.length() < 6 || confirm.length() > 15 ){
        //提示长度不准确
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("密码长度应为6-15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(confirm).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_CONFIRM_ERR);

    if(pass != confirm){
        //提示密码不匹配
        AddTipErr(TipErr::TIP_PWD_CONFIRM, tr("确认密码和密码不匹配"));
        return false;
    }else{
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
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
    json_obj["passwd"] = xorString(ui->password_edit->text());
    json_obj["confirm"] = xorString(ui->password_sure_edit->text());
    json_obj["varifycode"] = ui->verifycode_edit->text();
    httpmgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTEMOD);
}




void registerDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

void registerDialog::on_cancel_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

