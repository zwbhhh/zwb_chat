#pragma once
#include "gobal.h"
#include "HttpConnection.h"


class Cserver:public std::enable_shared_from_this<Cserver>
{
public:
	Cserver(boost::asio::io_context& ioc, unsigned short& port);
	void start();
	
private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;
	tcp::socket _socket;


};

