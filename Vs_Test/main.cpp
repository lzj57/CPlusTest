#pragma once
#pragma execution_character_set("utf-8")

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <thread>
#include <iostream>
using namespace std;
//#include "asyncTest.h"
//#include "ThreadTest.h"
//#include "PromiseTest.h"
//#include "Packaged_TaskTest.h"
//#include "Mutex_Lock_Test.h"
//#include "Condition_Var_Test.h"
#include "Atomic_Test.h"

int main() {
	
	//sum_func();
	//allocTest();
	//mulDoSom();
	//shared_fT();

	//thread_T1();
	//cout << "main---" << this_thread::get_id() << endl;

	//promise_T1();

	//task_T1(2,9);

	//mutex_T1();

	//con_Var_T2();

	atomic_T();

	//getchar();
	return 1;
}
