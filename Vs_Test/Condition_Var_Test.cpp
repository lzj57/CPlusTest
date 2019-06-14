#include "Condition_Var_Test.h"

#include <condition_variable>
#include <mutex>
#include <future>
#include <iostream>
#include <thread>
#include <queue>

//第一个例子
static bool readyFlag; //一个“用以存放待处理的数据”的对象或一个”用以表示条件真的满足了“的flag
static std::mutex readyMutex; //一个mutex
static std::condition_variable readyConVar; //一个condition variable

//第二个例子
std::queue<int> queue;
std::mutex queueMutex;
std::condition_variable queConVar;

void thread1() {
	std::cout << "<return>" << std::endl;
	std::cin.get();

	//signal that thread1 has prepared a conditon
	{
		std::lock_guard<std::mutex> lg(readyMutex);
		readyFlag = true;
	} // release lock
	readyConVar.notify_one();
}

void thread2() {
	//wait until thread1 is ready (readyFlag is true)
	{
		std::unique_lock<std::mutex> ul(readyMutex);
		// 下列效果是：wait()内部会不断调用第二实参，直到它返回true
		readyConVar.wait(ul, [] {return readyFlag; });
	}//release lock
	//do whatever shall happen after thread1 has prepared things
	std::cout << "done\n";
}

void con_Var_T1() {
	auto f1 = std::async(std::launch::async, thread1);
	auto f2 = std::async(std::launch::async, thread2);
	f1.get();
	f2.get();

}

//后台一直没有结束运行
void provider(int val) {
	//push different values
	for (int i = 0; i < 6; ++i) {
		std::lock_guard<std::mutex> lg(queueMutex);
		queue.push(val + i);
	}//release lock
	queConVar.notify_one();
	std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void consumer(int num) {
	//pop values if availabel (num identifies the consumer)
	while (true) {
		int val;
		{
			std::unique_lock<std::mutex> ul(queueMutex);
			queConVar.wait(ul, [] {return !queue.empty(); });
			//queConVar.wait(ul);
			val = queue.front();
			queue.pop();
		}//release lock
		std::cout << "consumer " << num << ": " << val << std::endl;
	} 
}

void con_Var_T2() {
	auto p1 = std::async(std::launch::async, provider, 100);
	auto p2 = std::async(std::launch::async, provider, 300);
	auto p3 = std::async(std::launch::async, provider, 500);

	auto c1 = std::async(std::launch::async, consumer,1);
	auto c2 = std::async(std::launch::async, consumer,2);

	p1.get();
	p2.get();
	p3.get();
	c1.get();
	c2.get();

	std::cout << "\ndone\n";
}