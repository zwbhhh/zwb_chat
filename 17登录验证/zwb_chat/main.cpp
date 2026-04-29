#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss("D:\\QT_llfc_project\\zwb_chat\\style\\stylesheet.qss");
    if( qss.open(QFile::ReadOnly)){
        qDebug("打开qss文件成功.....");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("打开qss文件失败");
    }

    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    // 拼接文件名
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path +
                                                   QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    ::gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://"+gate_host+":"+::gate_port;


    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
