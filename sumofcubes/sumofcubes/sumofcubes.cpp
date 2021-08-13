// sumofcubes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>

using namespace std;

unsigned int sum_cubes(unsigned int n) {
	int res = 0;
	for (int i = 1; i <= n; ++i) {
		res += (i * i * i);
	}
	return res;
}

void TestCases() {
	assert(sum_cubes(1) == 1);
	assert(sum_cubes(2) == 9);
	assert(sum_cubes(3) == 36);
	assert(sum_cubes(123) == 58155876);
}


int main()
{
    
	TestCases();
}



/*
Write a function that takes a positive integer n, sums all the cubed values from 1 to n, and returns that sum.

Assume that the input n will always be a positive integer.

Examples:
*/