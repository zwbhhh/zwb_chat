#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    void initHttpHandlers();
    void initHead();
    bool checkUserValid();
    bool checkPwdValid();
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    QMap<TipErr, QString> _tip_errs;
    void showTip(QString str, bool b_ok);
    bool enableBtn(bool enabled);
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    int _uid;
    QString _token;
public slots:
    void slot_forget_pwd();
signals:
    void switchSin();
    void switchReset();
    void sig_connect_tcp(ServerInfo);
private slots:
    void on_login_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    // bool checkPwdValid();
};

#endif // LOGINDIALOG_H
