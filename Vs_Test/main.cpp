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
//#include "Atomic_Test.h"
#include "FuncTest.h"


//测试malloc数据分配情况
void mallocTest() {
	int len1 = 100;
	int len2 = 200;
	int* t1 = (int*)malloc(sizeof(int) * len1);
	for (int i = 0; i < len1; ++i) {
		t1[i] = i + 1;
	}
	int* t2 = (int*)malloc(sizeof(int) * len2);
	for (int i = 0; i < len2; ++i) {
		t2[i] = i - 1;
	}
	int movLen = 10;
	t1 += movLen;
	len1 -= (movLen + movLen);
	for (int i = 0; i < len1; ++i) {
		cout << t1[i] << " ";
	}

	t1 -= movLen;
	free(t1);
	free(t2);
}

//文件读取测试
void fileRead() {
	FILE *fp;
	errno_t e = fopen_s(&fp, "1.txt", "r");
	if (e != 0) {
		cout << "打开失败！！" << endl;
		return;
	}
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *datas = (char*)malloc(len * sizeof(char));
	fread(datas, sizeof(char), len, fp);
	printf("%s\n", datas);
}

int main() {
	
	//fileRead();
	Calc res;
	int b = res.add(3, 5,1);
	int c = add_func(3, 3,10);
	cout << b << endl << c << endl;

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

	//atomic_T();

	//mallocTest();

	//getchar();
	return 1;
}
