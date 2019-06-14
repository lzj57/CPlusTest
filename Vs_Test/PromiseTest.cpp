#include "PromiseTest.h"
#include <thread>
#include <future>  //有promise的声明
#include <iostream>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>
#include <string>
/*
promise obj是async obj的配对兄弟，async可以利用get取数据，promise是让你提供数据（借由set_...()）
（可以通过promise来设置一个shared state，通过future来处理它）
promise 可以设置share state，future 可以获取share state,对promise调用get_future转换为future来获取promise的数据
不能即存储值有存储异常，企图这么做会导致std::future_error夹带差错码std::future_errc::promise_already_satisfied

shared state：通常实现为一个reference-counted object---当它被最后一个使用中释放时被销毁
*/

void doSomething(std::promise<std::string> &p) {
	try {
		std::cout << "read char('x' is exception):";
		char c = std::cin.get();
		if (c == 'x') {
			throw std::runtime_error(std::string("char ") + c + " read");
		}
		std::string s = std::string("char ") + c + " processed";
		//p.set_value(std::move(s)); //move，把s剪切返回
		p.set_value_at_thread_exit(std::move(s)); //
	}
	catch(...) {
	//<exception>辅助函数current_exption()将当前异常以类型std::exception_ptr生成处理，无异常nullptr
		//p.set_exception(std::current_exception()); 
		p.set_exception_at_thread_exit(std::current_exception());
	}
}


void promise_T1() {
	try {
		std::promise<std::string> p; 
		std::thread t(doSomething, std::ref(p)); //引用传递，保证状态得以被改变
		t.detach();
		std::future<std::string> f(p.get_future());
		std::cout << "result:" << f.get() << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << "EXCEPTION " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "EXCEPTION" << std::endl;
	}
}