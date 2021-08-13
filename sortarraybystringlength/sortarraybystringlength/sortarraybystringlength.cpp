// sortarraybystringlength.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
// #include <string>




class Kata
{
public:
	std::vector<std::string> sortByLength(std::vector<std::string> array);
	std::vector<std::string> sortByLengthQS(std::vector<std::string> array);
	
};


std::vector<std::string> Kata::sortByLength(std::vector<std::string> array)
{
	std::sort(array.begin(), array.end(), [](std::string a, std::string b) {return a.length() < b.length();});
	return array;
}


std::vector<std::string> Kata::sortByLengthQS(std::vector<std::string> array)
{
	// Using Bubble Sort
	const int len = array.size();

	std::string temp;
	for (int i = 1; i < len; ++i) {
		for (int j = i; j < len; ++j) {
			if (array[j].length() < array[j - 1].length()) {
				temp = array[j-1];
				array[j - 1] = array[j];
				array[j] = temp;
			}
		}
	}

	return array;

}



void Show(const std::vector<std::string>& arr)
{
	for (std::string s : arr) {
		std::cout << s << std::endl;
	}
}



int main()
{
	Kata* kata;
	kata = new Kata();

	std::vector<std::string> res;
	std::vector<std::string> arr = { "Telescopes", "Glasses", "Eyes", "Monocles" };


	// using standard std::sort (which I believe is QuickSort, nlog(n)
	std::cout << "std::sort()\n";
	res = kata->sortByLength(arr);
	Show(res);

	std::cout << std::endl << std::endl;
	// Using Bubble Sort, which is (n^2)
	std::cout << "Bubble Sort:\n";
	res = kata->sortByLengthQS(arr);
	Show(res);
	

	delete kata;
}
