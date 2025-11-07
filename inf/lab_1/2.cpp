// 2 вариант
//задача 2
#include <iostream>
using std::cout;
using std::cin;

int main()
{
	float fuel = 0;
	cout << "How much fuel in the car?\n";
	cin >> fuel;
	while (fuel >= 0) 
	{
		cout << "How many litres were put into the car?\n";
		cin >> fuel;
		if (fuel < 0)
		{
			break;
		}
	}
}