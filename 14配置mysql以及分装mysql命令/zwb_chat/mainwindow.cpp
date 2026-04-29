#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _register = new registerDialog(this);
    _login = new LoginDialog(this);
    setCentralWidget(_login);
    // _login->show();

    connect(_login, &LoginDialog::switchSin, this, &MainWindow::switchReg);

    //设置窗口自定义显示(作用就是把登录和注册界面加了this之后，能嵌套在主窗口页面)
    _login->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _register->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _register->hide();
}

MainWindow::~MainWindow()
{
    // delete ui;
    // if(_register){
    //     delete _register;
    //     _register = nullptr;
    // }
    // if(_login){
    //     delete _login;
    //     _login = nullptr;
    // }
}

void MainWindow::switchReg()
{
    setCentralWidget(_register);
    _login->hide();
    _register->show();
}
