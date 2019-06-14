#include "Packaged_TaskTest.h"
#include <future>
#include <iostream>
#include <chrono>
/*
需要处理一个后台task的成果，但不需要立刻启动task,可以用线程池控制何时启动以及多少个后台task同时运行
packaged_task<>定义于<future>内，task通常(非一定)启动于某一分离线程中
需要明确指出何时启动线程
*/

int compute(int x, int y) {
	for (int i = 0; i < 1000000000; ++i) {
		;// std::cout << 'A' + i << std::endl;
	}
	int res = x + y;
	std::cout << "result :" << res << std::endl;
	return res;
}

void task_T1(int x, int y) {
	std::packaged_task<int(int, int)> task(compute);
	std::future<int> f = task.get_future();
	
	std::future_status fs = f.wait_for(std::chrono::seconds(0));
	if (fs == std::future_status::ready) {
		std::cout << "ready\n";
	}else if (fs == std::future_status::deferred) {
		std::cout << "deffered\n";
	}
	else {
		std::cout << "timeout\n";
	}
	task(x, y);
	for (int i = 0; i < 10; ++i) {
		std::cout << 'a' + i << std::endl;
	}
	int res = f.get();
	std::cout << "\ndone" << std::endl;
}