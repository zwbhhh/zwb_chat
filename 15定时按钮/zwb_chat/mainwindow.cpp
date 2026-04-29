#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _register = new registerDialog(this);
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login);
    // _login->show();

    connect(_login, &LoginDialog::switchSin, this, &MainWindow::switchReg);


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
    _register = new registerDialog(this);
    //设置窗口自定义显示(作用就是把登录和注册界面加了this之后，能嵌套在主窗口页面)
    _register->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    //连接注册界面返回登录信号
    connect(_register, &registerDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(_register);
    _login->hide();
    _register->show();
}

void MainWindow::SlotSwitchLogin()
{
    // 创建一个CentralWidget，并将其设置为MainWindow的中心部件
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login);

    _register->hide();
    _login->show();

    // 连接登录界面注册信号
    connect(_login, &LoginDialog::switchSin, this, &MainWindow::switchReg);


}
