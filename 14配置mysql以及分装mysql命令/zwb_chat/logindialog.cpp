#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->register_2, &QPushButton::clicked, this, &LoginDialog::switchSin);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
