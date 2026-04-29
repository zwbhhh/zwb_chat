#include "Cserver.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"
Cserver::Cserver(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {

}

void Cserver::start()
{
	auto self = shared_from_this();
	auto& in_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(in_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			//出错则放弃这个连接，继续监听新链接
			if (ec) {
				self->start();
				return;
			}
			//处理新链接，创建HpptConnection类管理新连接
			//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			//线程池来启动
			new_con->Start();

			//继续监听
			self->start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->start();
		}
		});
}