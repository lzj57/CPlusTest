#include "Atomic_Test.h"

#include <future>
#include <iostream>
#include <thread>
#include <queue>
#include <atomic>


long data;
std::atomic<bool> readyFlag(false);
void provider() {
	//after reading a character
	std::cout << "<retur>" << std::endl;
	std::cin.get();

	data = 42;
	//and signal readiness
	readyFlag.store(true);
}

void consumer() {
	//wait for readliness and do something else
	while (!readyFlag.load()) {
		std::cout.put('.').flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	//and process provided data
	std::cout << "\nvalue" << data << std::endl;
}

void atomic_T() {
	auto p1 = std::async(std::launch::async, provider);
	auto p2 = std::async(std::launch::async, consumer);

	p1.get();
	p2.get();
}


//static std::atomic<bool> readyFlag(false);
//std::atomic<bool> readyFlag;
//static std::mutex readyMutex; //一个mutex
//static std::condition_variable readyConVar;
//
//void thread_ato1() {
//	std::cout << "<return>" << std::endl;
//	std::cin.get();
//	std::atomic_init(&readyFlag, false);
//
//	readyFlag.store(true);
//	
//	readyConVar.notify_one();
//}
//
//void thread_ato2() {
//	//wait until thread1 is ready (readyFlag is true)
//	{
//		std::unique_lock<std::mutex> l(readyMutex);
//		readyConVar.wait(l, [] {return readyFlag.load(); });
//	}
//	
//	//do whatever shall happen after thread1 has prepared things
//	std::cout << "done\n";
//}
//
//void atomic_T1() {
//	auto f1 = std::async(std::launch::async, thread_ato1);
//	auto f2 = std::async(std::launch::async, thread_ato2);
//	f1.get();
//	f2.get();
//
//}

