#include "asyncTest.h"
#include <future>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>

#include <list>

//#include <sstream> //获取线程的时候调用

#include <stdexcept>

using namespace std;

int doSomething(char c) {
	default_random_engine dre(c);
	uniform_int_distribution<int> id(10, 1000);
	//获取当前线程
	//std::ostringstream ss;
	//ss << this_thread::get_id();
	//string str = ss.str();
	for (int i = 0; i < 10; ++i) {
		this_thread::sleep_for(chrono::milliseconds(id(dre)));
		//cout << str << "--";
		cout.put(c).flush();
	}
	
	
	return c;
}

int func1() {
	return doSomething('.');
}
int func2() {
	return doSomething('+');
}

/*
* async 尝试将获得的函数立刻异步启动于一个分离线程内（函数，对象，lambda）
返回一个future object内：
1.允许取得async未来的结果--可能是个返回值，也可能是个异常；
2.确保函数或快或慢的被调用

launch(发射)策略，launch::async确保函数异步执行
利用launch::deferrred强制延缓执行，延缓到调用get时
*/
void sum_func() {
	cout << "starting func1() and func2()" << endl;
	// 后台调用，可能直接启动了一个进程，也可能在get的时候执行
	//future<int> result1(async(func1)); //future<int> 可以写成auto
	//launch(发射)策略，launch::async确保函数异步执行
	//future<int> result1 = async(launch::async, func1);
	//利用launch::deferrred强制延缓执行，延缓到调用get时
	future<int> result1(async(launch::deferred, func1));
	int result2 = func2();
	//int result = result1.get() + result2;
	int result = result1.get() + result2;
	cout << "add result:" << result << endl; //result.get()保证函数一定被调用
}


void task1() {
	list<int> v;
	while (true) {
		for (int i = 0; i < 100000; ++i) {
			v.push_back(i);
		}
		cout.put('.').flush();
	}
}

/*
future：
	get一次后，将处于无效状态，通过valid（）来检测
	调用wait(),可以强制启动future这一线程的运行并等待其终止。

	wait_for()，等待有限时间
	wait_until(),等待直达某个特定时间点
	wait_for()和wait_deferred()返回以下三种东西：
		future_status::deferred,async延缓了操作
		future_status::timeout，操作未完成，wait已逾期
		future_status::ready，操作完成
*/
void allocTest() {
	cout << "starting 2 tastks" << endl;
	cout << "- task1:process endless loop of memory consumption" << endl;
	cout << "-task2:wait for <retur> and then for task1" << endl;
	auto f1 = async(task1);
	//future_status s = f1.wait_for(chrono::seconds(3)); // 等待10s
	future_status s = f1.wait_until(chrono::system_clock::now() + chrono::minutes(1));
	cout << "\n wait fo the end of task1:" << endl;
	//cin.get();
	try {
		f1.get();
		cout << "Correct\n";
	}
	catch (const exception& e) {
		cerr << "EXCEPTION:" << e.what() << endl;
	}
}


int quickComputation();
int accurateComputation();
future<int> f; //f不能在baseRIT函数里，会因为f未执行完导致f的析构函数停滞直到异步操作结束

//this_thread::yield(),睡眠一小段时间

int baseResultInTime() {
	auto tp = chrono::system_clock::now() + chrono::minutes(1);
	f = async(launch::async, accurateComputation);
	int guess = quickComputation();
	future_status s = f.wait_until(tp);
	if (s == std::future_status::ready) {
		return f.get();
	}
	else {
		return guess ;
	}
}

int quickComputation() {
	return 1;
}
int accurateComputation() {
	return 0;
}


/*
async可传入成员函数
1.成员函数的指针（&Class::func）
2.指向object的reference或pointer(classObj);
3.成员函数的实参
*/
void mulDoSom() {
	char c = '?';
	//auto f1 = async([=] {doSomething(c); });
	auto f1 = async(doSomething, c); 
	//采用引用的方式，ref(c),[&]可能在后台启用时就无效
	auto f2 = async([] {doSomething('+'); });
	if (f1.wait_for(chrono::seconds(0)) != future_status::deferred || \
		f2.wait_for(chrono::seconds(0)) != future_status::deferred) {
		while (f1.wait_for(chrono::seconds(0)) != future_status::ready && \
			f2.wait_for(chrono::seconds(0)) != future_status::ready) {
			this_thread::yield();
		}
	}
	cout.put('\n').flush();
	try {
		f1.get();
		f2.get();
	}
	catch (const exception& e) {
		cout << "\nEXCEPTION:" << e.what() << endl;
	}
	cout << "\ndone" << endl;
}


/*
future处理结果只能get一次，第二次调用一般是抛出一个future_error
shared_futrue---可以多次调用get,get的每次结果一致
*/

int queryNumber() {
	cout << "read number:";
	int num;
	cin >> num;
	if (!cin) {
		throw runtime_error("no number read");
	}
	return num;
}

void doSomething1(char c, shared_future<int> f) {
	try {
		int num = f.get();
		for (int i = 0; i < num; ++i) {
			this_thread::sleep_for(chrono::milliseconds(100));
			cout.put(c).flush();
		}	
	}
	catch (const exception& e) {
		cerr << "EXCEPTION in thread" << this_thread::get_id() \
			<< ":" << e.what() << endl;
	}
}

/*
auto代替shared_future---auto f = async(queryNumber).shared();
所有shared future object共享shared state(async产生)
*/
void shared_fT() {
	try {
		//shared_future<int> f = async(queryNumber);
		auto f = async(queryNumber).share();
		auto f1 = async(launch::async, doSomething1, '.', f);
		auto f2 = async(launch::async, doSomething1, '+', f);
		auto f3 = async(launch::async, doSomething1, '*', f);
		f1.get();
		f2.get();
		f3.get();
	}
	catch (const exception& e) {
		cout << "\nException" << e.what() << endl;
	}
	cout << "\ndone" << endl;
}