#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
/*****************************************************
* @file     mainwindow.h
* @brief    主界面 Function
*
* @author   张伟标
* @data     2026/04/11
* @history
******************************************************/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    LoginDialog* _login;
    registerDialog* _register;
    ResetDialog* _reset_dlg;
private slots:
    void switchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void SlotSwitchLogin2();
};
#endif // MAINWINDOW_H
