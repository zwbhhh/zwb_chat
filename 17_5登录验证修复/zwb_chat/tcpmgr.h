#ifndef TCPMGR_H
#define TCPMGR_H
#include <QtcpSocket>
#include "signalton.h"
#include "global.h"
#include <functional>
#include <QObject>
class TcpMgr:public QObject, public Signalton<TcpMgr>,
               public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    ~TcpMgr();
private:
    friend class Signalton<TcpMgr>;
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);
    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;

public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray dataBytes);

signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_switch_chatdlg();
    void sig_login_failed(int);
    void sig_connection_closed();
    void sig_swich_chatdlg();

};

#endif // TCPMGR_H
