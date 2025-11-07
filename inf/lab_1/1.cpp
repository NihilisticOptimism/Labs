#include <iostream>
//вариант 2

//задача 1
using std::cout;
using std::cin;

int main()
{
	int need = 20, rec = 50;
	float fuel = 0;
	cout << "How much fuel in the car?\n";
	cin >> fuel;
	if (fuel > rec)
	{
		cout << "There's enough fuel";
	}
	else if (fuel < need)
	{
		cout << "Fuel is running out, urgently refuel!";
	}
	else
	{
		cout << "Refueling is recommended";
	}
}