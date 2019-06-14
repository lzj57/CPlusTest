#include "Mutex_Lock_Test.h"

#include <mutex>
#include <iostream>
#include <string>
#include <future>

std::mutex printMutex;
//std::recursive_mutex printMutex;

void print(const std::string &s) {
	std::lock_guard<std::mutex> l(printMutex); //加入,std::adopt_lock 出现 "unlock of unowned mutex "
	//std::lock_guard<std::recursive_mutex> l(printMutex, std::adopt_lock);

	for (char c : s) {
		std::cout.put(c);
	}
	std::cout << std::endl;
}

void mutex_T1() { 
	auto f1 = std::async(std::launch::async, print, "Hello from first thread");
	auto f2 = std::async(std::launch::async, print, "Hello from second thread");
	print("Hello for T1 thread");
// 测试中出现"mutex destroy while busy",加入等待时间，避免后台运行过程中，程序结束
	//getchar();
	f1.get();
	f2.get();
}