#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::getline;
using std::istringstream;
using std::ios;


class dish {
    public:
        string name;
        string type;
        double price;
};

vector<dish> readFile(ifstream& inFile){
	vector<dish> dishes;
	string name;
	string type;
	double price;
	string line;
	while(getline(inFile, line)){
		istringstream stream(line);
		getline(stream, name, ';');
		getline(stream, type, ';');
		stream >> price;
		dishes.push_back({name, type, price});
	}
	return dishes;
}

void output(const vector<dish>& dishes){
	for(dish i : dishes){
		cout << i.name << "; " << i.type << "; " << i.price << endl;
	}
}

void writeFile(const vector<dish>& dishes){
	string name;
	string type;
	double price;
	ofstream outFile("menu.txt", ios::out);
	if(outFile.is_open()){
		for(size_t i = 0; i < dishes.size(); i++){
			name = dishes[i].name;
			type = dishes[i].type;
			price = dishes[i].price;
			outFile << name << ";" << type << ";" << price << endl;
		}
		outFile.close();
	} else{
		cout << "File opening error" << endl;
	}
}

bool isType(const string& input){
	vector<string> Types = {"Snacks", "Main dishes", "Pudding", "Drinks", "ForKids"};
    for(string type : Types){
		if (input == type) {
            return true;
        } 
	}
    return false;
}


void input(ofstream& outFile){
	string line;
	string name;
	string type;
	double price;
	cout << "Enter in the following order: name, type ('Snacks, MainDishes, Pudding, Drinks, ForKids'), cost / just press enter:" << endl;
	while(true){
		getline(cin, line);
		istringstream stream(line);
		if(line.empty()) break;
		getline(stream, name, ' ');
		getline(stream, type, ' ');
		while(!isType(type)){
			cout << "Dish type error. Try again: ";
			getline(cin, type);
	}
		stream >> price;
		while(price <= 0){
			cout << "Price cannot be negative. Try again: ";
			cin >> price;
		}
		outFile << name << ";" << type << ";" << price << endl;
		cout << "Dish successfully added" << endl;
	}
}

bool dishSearch(ifstream& inFile, const string& name){
	string name_dish;
	string line;
	string type;
	double price;
	bool flag = true;
	while(getline(inFile, line)){
		if(line.find(name) != string::npos){
			istringstream stream(line);
			getline(stream, name_dish, ';');
			cout << "Name of the dish: " << name_dish << "\n";
			getline(stream, type, ';');
			cout << "Type of the dish: " << type << "\n";
			stream >> price;
			cout << "Price of the dish: " << price << "\n";
			flag = false;
			break;
		}
	}
	if(flag){
		cout << "Eror: dish not found";
	}
	return flag;
}

void sortByType(vector<dish>& dishes){
	sort(dishes.begin(), dishes.end(), [](const dish& a, const dish& b){return a.type < b.type;});
	output(dishes);
	writeFile(dishes);
}

void sortByPrice(vector<dish>& dishes){
	sort(dishes.begin(), dishes.end(), [](const dish a, const dish b){return a.price < b.price;});
	output(dishes);
	writeFile(dishes);
}



void addFile(ofstream& outFile){
	string line;
	string name;
	string type;
	double price;
	cout << "Enter in the following order: name, type, cost (or empty line for exiting):" << endl;
	getline(cin, line);
	istringstream stream(line);
	getline(stream, name, ',');
	getline(stream, type, ',');
	while(!isType(type)){
		cout << "Dish type error. Try again: ";
		getline(cin, type);
	}
	stream >> price;
	while(price <= 0){
		cout << "Price cannot be negative. Try again: ";
		cin >> price;
	}
	outFile << name << ";" << type << ";" << price << endl;
	cout << "Dish successfully added" << endl;
}

void query(ifstream& inFile, double price_lim){
	vector<dish> dishes;
	string name;
	string type;
	double price;
	string line;
	while(getline(inFile, line)){
		istringstream stream(line);
		getline(stream, name, ';');
		getline(stream, type, ';');
		stream >> price;
		dishes.push_back({name, type, price});
	}
	cout << "Dishes with the cost below " << price_lim << endl;
	for(dish i : dishes){
		if(i.price <= price_lim){
			cout << i.name << ";" << i.type << ";" << i.price << endl;
		}
	}
}

void menu(){
	cout << "Action menu:" << endl;
	cout << "1.Add dish" << endl;
	cout << "2.Find dish" << endl;
	cout << "3.Sort by price" << endl;
	cout << "4.Sort by type" << endl;
	cout << "5.Lower than price" << endl;
	cout << "6.Data output" << endl;
	cout << "0.Menu exit" << endl;
	cout << "Choose one of the following: ";
}

int main()
{
    cout << isType("ForKids") << endl;
	cout << "Create menu" << endl;
	ofstream outFile("menu.txt");
	if(outFile.is_open()){
		input(outFile);
		outFile.close();
	} else{
		cout << "Failed to open file for recording" << endl;
	}
	int i;
	menu();
	cin >> i;
	cin.ignore();

	ofstream outTemp("output.txt", ios::out | ios::app);
	switch (i) {
		case 1:{
			ofstream outFile("menu.txt", ios::app);
			if(outFile.is_open()){
				addFile(outFile);
				outTemp << "Case 1 executed" << endl;
				outFile.close();
			} else{
				cout << "Failed to open file for recording" << endl;
				outTemp << "Case 1 falied" << endl;
			}
			outTemp.close();
			break;
		}

		case 2:{
			ifstream inFile("menu.txt", ios::in);
			if(inFile.is_open()){
				string name;
				cout << "Enter the name of the dish: ";
				getline(cin, name);
				if(!dishSearch(inFile, name)){
					outTemp << "Case 2 executed" << endl;
				}
				else{
					outTemp << "Case 2 failed" << endl;
				}

				inFile.close();
			} else{
				cout << "Failed to open file for recording" << endl;
				outTemp << "Case 2 failed" << endl;
			}
			outTemp.close();
			break;
		}

		case 3:{
			vector<dish> dishes;
			ifstream inFile("menu.txt", ios::in);
			if(inFile.is_open()){
				dishes = readFile(inFile);
				sortByPrice(dishes);
				outTemp << "Case 3 executed" << endl;
				inFile.close();
			} else{
				cout << "Failed to open file for recording" << endl;
				outTemp << "Case 3 failed" << endl;
			}
			outTemp.close();
			break;
		}

		case 4:{
			vector<dish> dishes;
			ifstream inFile("menu.txt", ios::in);
			if(inFile.is_open()){
				dishes = readFile(inFile);
				sortByType(dishes);
				outTemp << "Case 4 executed" << endl;
				inFile.close();
			} else{
				cout << "Failed to open file for recording" << endl;
				outTemp << "Case 4 executed" << endl;
			}
			outTemp.close();
			break;
		}

		case 5:{
			ifstream inFile("menu.txt", ios::in);
			double price_lim;
			cout << "Enter value: ";
			cin >> price_lim;
			if(price_lim < 0){
				cout << "Value cannot be negative" << endl;
				outTemp << "Case 5 failed" << endl;
				outTemp.close();
				break;
			}
			if(inFile.is_open()){
				cout << endl;
				query(inFile, price_lim);
				outTemp << "Case 5 executed" << endl;
				inFile.close();
			} else{
				cout << "Failed to open file for recording" << endl;
				outTemp << "Case 5 failed" << endl;
			}
			outTemp.close();
			break;
		}

		case 6:{
			vector<dish> dishes;
			ifstream inFile("menu.txt", ios::in);
			if(inFile.is_open()){
				dishes = readFile(inFile);
				output(dishes);
				outTemp << "Case 6 executed" << endl;
				inFile.close();
			} else{
				cout << "Failed to open file for recording" << endl;
				outTemp << "Case 1 failed" << endl;
			}
			outTemp.close();
			break;
		}

		case 0:{
			cout << "Exiting..." << endl;
			break;
		}

		default:{
			cout << "Incorrect choice." << endl;
		}
	}
	return 0;
}