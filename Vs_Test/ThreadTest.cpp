#include "ThreadTest.h"
#include <thread>
#include <chrono>
#include <random>
#include <exception>
#include <iostream>
using namespace std;

void doSomthing(int num,int c) {
	try {
		default_random_engine dre(42 * c);
		uniform_int_distribution<int> id(10, 1000);
		for (int i = 0; i < num; ++i) {
			this_thread::sleep_for(chrono::milliseconds(id(dre)));
			cout.put(c).flush();
		}
		cout << endl;
	}
	catch (const exception& e) {
		cerr << "Thread_EXCEPTION" << this_thread::get_id() << "--" << e.what() << endl;
	}
	catch (...) {
		cerr << "Thread_EXCEPTION" << this_thread::get_id() << endl;
	}
}

/*
thread可以传入函数，函数对象，lamba
获取thread id：thread::id(),thread::get_id(),可能在动态生成时才有这些id，而不是每次启动就生成了。回收的线程id可能会被拿来重复使用
thread与高级接口async相比，不提供：
	class thread 没有发射策略，永远试着将函数启动与一个新线程，无法做到会抛出system_error并带有差错码resource_unavailable_try_again
	没有接口可以处理线程结束，只可以获得线程的ID
	如果发生异常，但未捕捉于线程之内，程序会立刻中止并调用std::terminate(),若想将异常传播至线程外的某个context，必须使用exception_ptr
	必须声明是否想要等待线程结束(join())或打算将它自母体卸离使它运行后台不受任何控制（detach()），如果在thread obj结束前不这么做或如果发生了一次move assignment,会中止并调用std::terminate()
	如果让线程运行于后台而main结束了，所有线程会被硬性的终止

detach
	无法得知它是否运行以及运行了多久，确保不要使用访问寿命已结束的object
	程序退离，detached的线程可能还在运行
detach使用的一般规则：
	1.宁可只访问local copy
	2.针对global或statci obj
		确保他们销毁在detached threadd结束或不在访问之后，一般用conditon variable，让线程发信号说他们已经结束了
		调用quick_exit()结束程序，这个函数是为了以“不调用global和static obj的析构函数”结束程序
经验法则：终止detached thread的唯一安全方法就是搭配“...at_thread_exit()”函数群（理想中使用notify_all_~）中的某一个，这会强制main thread等待detached thread真正结束

*/
void thread_T1() {
	try {//启动线程可能抛出一个夹带差错码resource_unavailable_try_again的异常std::system_error
		thread t1(doSomthing, 5, '+');
		cout << "- started fg thread" << t1.get_id() << endl;
		for (int i = 0; i < 5; ++i) {
			thread t(doSomthing, 10, 'a' + i);
			cout << "- detach started bg thread" << t.get_id() << endl;
			t.detach();
		}
		cin.get();
		cout << "--join fg thread" << t1.get_id() << endl;
		t1.join();
	}
	catch (const exception& e) {
		cerr << "EXCEPTION:" << e.what() << endl;
	}
}



