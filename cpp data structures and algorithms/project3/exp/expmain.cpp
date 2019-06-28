// expmain.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Do whatever you'd like here.  This is intended to allow you to experiment
// with your code, outside of the context of the broader program or Google
// Test.
#include <memory>
#include <iostream>
#include <algorithm>
int main()
{
	std::unique_ptr<int> x{new int{1}};
	std::unique_ptr<int> y{new int{2}};

	std::cout << x.get() << " and "<< y.get() <<std::endl;

	std::cout << "swapping: " << std::endl;
	std::swap(x, y);

	


    return 0;
}

