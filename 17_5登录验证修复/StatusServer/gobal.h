#pragma once
#include<boost/beast/http.hpp>
#include<boost/beast.hpp>
#include<boost/asio.hpp>
#include<memory>   //enable_shared_from_this函数的头文件
#include <iostream>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "hiredis.h"
#include <assert.h>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  
	RPCFailed = 1002,  
	VarifyExpired = 1003,
	VarifyCodeErr = 1004,
	UserExist = 1005, 
	PasswdErr = 1006, 
	EmailNotMatch = 1007,
	PasswdUpFailed = 1008, 
	PasswdInvalid = 1009,  
	TokenInvalid = 1010,   
	UidInvalid = 1011,  
	RPCGetFailed = 1012,
};

// Defer类
class Defer {
public:
	// 接受一个lambda表达式或者函数指针
	Defer(std::function<void()> func) : func_(func) {}

	// 析构函数中执行传入的函数
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define CODEPREFIX "code_"
#define USERIPPREFIX  "uip_"
#define USERTOKENPREFIX  "utoken_"
#define IPCOUNTPREFIX  "ipcount_"
#define USER_BASE_INFO "ubaseinfo_"
#define LOGIN_COUNT  "logincount"
#define LOCK_COUNT "lockcount"