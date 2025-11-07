//вариант 2
#include <iostream>
using std::cout;
using std::cin;

void bubble(int arr[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1 - i; j++)
		{
			int n1 = arr[j];
			int n2 = arr[j + 1];
			if (n1 < n2)
			{
				arr[j] = n2;
				arr[j + 1] = n1;
			}
		}

	}
}


int main()
{
	const int len = 10;
	int numbers[len];
	int i = 0;
	cout << "Input 10 elements of the array\n";
	for (int i = 0; i < len; i++)
	{
		cin >> numbers[i];
	}
	int su = 0;
	cout << "your array: ";
	for (int i = 0; i < len; i++)
	{
		cout << numbers[i] << " ";
		su = su + numbers[i];
	}
	cout << "\n";
	cout << "the sum: " << su << "\n";
	bubble(numbers, len);
	cout << "biggest number: " << numbers[0] << "\n";
	cout << "sorted array: ";
	for (int i = 0; i < len; i++)
	{
		cout << numbers[i] << " ";
	}
}