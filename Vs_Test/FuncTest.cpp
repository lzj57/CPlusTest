#include "FuncTest.h"



Calc::Calc() {

}

int Calc::add(int a, int b, int c) {
	int res = a + b + c;
	return res;
}

int add_func(int a, int b, int c) {
	int res = a + b + c;
	return res;
}