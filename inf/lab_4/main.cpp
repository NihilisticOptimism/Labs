#include <iostream>
#include <pqxx/pqxx>
#include <istream>
#include <regex>
#include <fstream>
#include <chrono>
#include <sstream>


using std::cout;
using std::string;
using std::endl;
using std::cerr;
using std::cout;
using std::exception;
using std::cin;

class Category {
private:
	int id;
	string name;
public:

	Category(const string& n) : name(n) {};

	void addCategory(pqxx::connection& conn) {
		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"INSERT INTO categories (category_name) VALUES (" +
				txn.quote(name) +
				") RETURNING id"
			);

			id = res[0]["id"].as<int>();

			txn.commit();

			cout << "Category added. ID = " << id << endl;
		}
		catch (const exception& e) {
			cerr << "Error in adding category: " << e.what() << endl;
		}
	}

	static void allCategories(pqxx::connection& conn) {

		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec("SELECT * FROM categories ORDER BY id");

			cout << "Available categories:" << endl;

			for (const auto& row : res) {
				cout << "ID: " << row["id"].as<int>()
					<< ", category_name: " << row["category_name"].c_str()
					<< endl;
			}
		}
		catch (const exception& e) {
			cerr << "Error in trying to read categories: " << e.what() << endl;
		}
	}

	static void totalRevenue(pqxx::connection& conn) {

		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"SELECT\n"
				"c.id,\n"
				"c.category_name,\n"
				"COALESCE(SUM(d.price * o.quantity), 0) AS total_price\n"
				"FROM categories c\n"
				"LEFT JOIN dishes d ON c.id = d.category_id\n"
				"LEFT JOIN orders o ON d.id = o.dish_id\n"
				"GROUP BY c.id, c.category_name\n"
				"ORDER BY c.category_name;"
			);

			cout << "Total revenue by category:" << endl;

			for (const auto& row : res) {
				cout << "ID: " << row["id"].as<int>()
					<< ", category_name: " << row["category_name"].c_str()
					<< ", total_price: " << row["total_price"].as<int>()
					<< endl;
			}
		}
		catch (const exception& e) {
			cerr << "Error in reading orders: " << e.what() << endl;
		}
	}

	int getId() {
		return id;
	}

	string getName() {
		return name;
	}
};


class Dish {
private:
	int id;
	string name;
	int category_id;
	double price;

public:

	Dish(const string& n, int id, double p) : name(n), category_id(id), price(p) {};

	void addDish(pqxx::connection& conn) {
		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"INSERT INTO dishes (name, category_id, price) VALUES ("
				+ txn.quote(name) + ", "
				+ to_string(category_id) + ", "
				+ to_string(price) +
				") RETURNING id"
			);
			id = res[0]["id"].as<int>();

			txn.commit();

			cout << "Dish added. ID = " << id << endl;

		}
		catch (const exception& e) {
			cerr << "Eror in addding dish: " << e.what() << endl;
		}
	}

	static void showDishes(pqxx::connection& conn) {
		try {

			pqxx::work txn(conn);

			pqxx::result res = txn.exec("SELECT * FROM dishes ORDER BY id");

			cout << "All dishes:" << endl;

			for (const auto& row : res) {
				cout << "ID: " << row["id"].as<int>()
					<< ", Name: " << row["name"].c_str()
					<< ", Category ID: " << row["category_id"].as<int>()
					<< ", Price: " << row["price"].as<double>()
					<< endl;
			}

			txn.commit();
		}
		catch (const exception& e) {
			cerr << "Error in displaying dishes: " << e.what() << endl;
		}
	}

	static void allDish(pqxx::connection& conn) {

		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec("SELECT * FROM dishes ORDER BY id");

			cout << "Available dishes:" << endl;

			for (const auto& row : res) {
				cout << "ID: " << row["id"].as<int>()
					<< ", name: " << row["name"].c_str()
					<< endl;
			}
		}
		catch (const exception& e) {
			cerr << "Error in reading dishes: " << e.what() << endl;
		}
	}

	static void top3Dish(pqxx::connection& conn) {
		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"SELECT\n"
				"d.id,\n"
				"d.name,\n"
				"COALESCE(SUM(o.quantity), 0) AS quantity\n"
				"FROM dishes d\n"
				"LEFT JOIN orders o ON d.id = o.dish_id\n"
				"GROUP BY d.id, d.name\n"
				"ORDER BY quantity DESC\n"
				"LIMIT 3;"
			);

			for (const auto& row : res) {
				cout << "Name: " << row["name"].c_str() << ", Quantity: " << row["quantity"].as<int>() << endl;
			}

			txn.commit();
		}
		catch (const exception& e) {
			cerr << "Error in reading orders: " << e.what() << endl;
		}
	}

	int getId() {
		return id;
	}
};


class Order {
private:

	int order_id;
	int dish_id;
	string order_date;
	int quantity;
public:

	Order(int id, const string& date, int q) : dish_id(id), order_date(date), quantity(q) {};

	void addOrder(pqxx::connection& conn) {

		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec("INSERT INTO orders (dish_id, order_date, quantity) VALUES ("
				+ to_string(dish_id) + ", "
				+ txn.quote(order_date) + ", "
				+ to_string(quantity) +
				") RETURNING order_id"
			);

			order_id = res[0]["order_id"].as<int>();

			cout << "Order added. ID = " << order_id << endl;

			txn.commit();

		}
		catch (const exception& e) {
			cerr << "Error in adding order: " << e.what() << endl;
		}
	}

	static void showOrders(pqxx::connection& conn) {
		try {

			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"SELECT\n"
				"	o.*,\n"
				"	(o.quantity * d.price) AS total_price\n"
				"FROM orders o\n"
				"JOIN dishes d ON o.dish_id = d.id\n"
				"ORDER BY order_date;"
			);

			txn.commit();

			cout << "All orders:" << endl;

			for (const auto& row : res) {
				cout << "ID: " << row["order_id"].as<int>()
					<< ", Dish_ID: " << row["dish_id"].as<int>()
					<< ", Order_date: " << row["order_date"].c_str()
					<< ", Quantity: " << row["quantity"].as<int>()
					<< ", Total_price: " << row["total_price"].as<double>()
					<< endl;
			}



		}
		catch (const exception& e) {
			cerr << "Error in displaying dishes: " << e.what() << endl;
		}
	}

	static void averageCost(pqxx::connection& conn) {

		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"SELECT\n"
				"AVG(o.quantity * d.price) AS average_cost\n"
				"FROM orders o\n"
				"JOIN dishes d ON d.id = o.dish_id;"
			);

			cout << "Average order cost:" << endl;

			cout << res[0]["average_cost"].as<double>() << endl;
		}
		catch (const exception& e) {
			cerr << "Error in reading orders: " << e.what() << endl;
		}
	}

	static void countOrders(pqxx::connection& conn) {

		try {
			pqxx::work txn(conn);

			pqxx::result res = txn.exec(
				"SELECT\n"
				"d.id,\n"
				"d.name,\n"
				"COUNT(o.order_id) AS total_count\n"
				"FROM dishes d\n"
				"LEFT JOIN orders o ON o.dish_id = d.id\n"
				"GROUP BY d.id, d.name\n"
				"ORDER BY d.id ASC;\n"
			);

			cout << "Amount of orders per dish:" << endl;

			for (const auto& row : res) {
				cout << "ID: " << row["id"].as<int>()
					<< ", name: " << row["name"].c_str()
					<< ", total_count: " << row["total_count"].as<int>()
					<< endl;
			}
		}
		catch (const exception& e) {
			cerr << "Error in reading orders: " << e.what() << endl;
		}
	}

	int getId() {
		return order_id;
	}
};


class MainCourse : public Category {
public:
	string getType() {
		return getName();
	}
};

class Dessert : public Category {
public:
	string getType() {
		return getName();
	}
};

template <typename T1, typename T2>
class Pair {
public:
	T1 first;
	T2 second;

	Pair(T1 f, T2 s) : first(f), second(s) {}
};


bool isId(pqxx::connection& conn, int id, const string& table) {

	bool f;

	try {

		pqxx::work txn(conn);

		pqxx::result res = txn.exec("SELECT 1 FROM " + table + " WHERE id = " + to_string(id));

		f = !res.empty();
	}
	catch (const exception& e) {
		cerr << "Error in checking ID: " << e.what() << endl;
	}

	return f;
}

bool isValidDate(const string& date) {

	regex date_regex(R"(\d{4}-\d{2}-\d{2})");
	if (!regex_match(date, date_regex)) return false;

	int year = stoi(date.substr(0, 4));
	int month = stoi(date.substr(5, 2));
	int day = stoi(date.substr(8, 2));

	if (month < 1 || month > 12) return false;

	int days_in_month[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		days_in_month[2] = 29;

	if (day < 1 || day > days_in_month[month]) return false;

	return true;
}

void logAction(ofstream& file, const string& text) {
	auto now = chrono::system_clock::now();
	time_t now_c = chrono::system_clock::to_time_t(now);
	tm now_tm = *localtime(&now_c);
	file << text << " " << put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << endl;
}

void Menu() {
	cout << "===CHOOSE ACTION===" << endl;
	cout << "1.Add category" << endl;
	cout << "2.Add dish" << endl;
	cout << "3.Add order" << endl;
	cout << "4.See all dishes" << endl;
	cout << "5.See all orders" << endl;
	cout << "6.Display total revenue by category" << endl;
	cout << "7.Display the 3 most sold dishes" << endl;
	cout << "8.Display the average order cost" << endl;
	cout << "9.Display the amount of orders per each dish" << endl;
	cout << "10.Exit" << endl;
	cout << "Your choice: ";
}

int main() {
	int choice = 0;

	ofstream outFile("log.txt", ios::app);
	if (!outFile.is_open()) {
		cerr << "Error! Cannot open file for logging" << endl;
	}
	else {

		try {
			string conninfo = "host=localhost port=5432 dbname=restaurant_db user=postgres password=***";

			pqxx::connection conn(conninfo);

			if (conn.is_open()) {
				cout << "Connected to database: " << conn.dbname() << endl;
			}
			else {
				cerr << "Failed to connect to the database." << endl;
				return 1;
			}

			do {
				cout << endl;
				Menu();
				cin >> choice;
				cin.ignore();

				switch (choice) {
				case (1): {
					string name;

					Category::allCategories(conn);
					cout << "Input category name: ";
					getline(cin, name);

					Category newCategory(name);
					newCategory.addCategory(conn);
					ostringstream ss;
					ss << "Added new category with ID = " << newCategory.getId() << " ";
					logAction(outFile, ss.str());
					break;
				}
				case (2): {
					string name;
					int id;
					double price;

					cout << "Input dish name: ";
					getline(cin, name);
					Category::allCategories(conn);
					cout << "Input category id: ";
					cin >> id;
					while (!isId(conn, id, "categories")) {
						cout << "Error! Category not found" << endl;
						cout << endl;
						Category::allCategories(conn);
						cout << "Input category id: ";
						cin >> id;
					}
					cout << "Input cost: ";
					cin >> price;
					while (price < 0) {
						cout << "Error! Cost cannot be negative" << endl;
						cout << "Input cost: ";
						cin >> price;
					}

					Dish newDish(name, id, price);
					newDish.addDish(conn);
					ostringstream ss;
					ss << "Added new dish with ID = " << newDish.getId() << " ";
					logAction(outFile, ss.str());
					break;
				}
				case (3): {
					int dish_id;
					string order_date;
					int quantity;

					Dish::allDish(conn);
					cout << "Input dish id: ";
					cin >> dish_id;
					while (!isId(conn, dish_id, "dishes")) {
						cout << "Error! Dish not found" << endl;
						cout << endl;
						Dish::allDish(conn);
						cout << "Input dish id: ";
						cin >> dish_id;
					}
					cin.ignore();
					cout << "Input order date in format 'YYYY-MM-DD': ";
					getline(cin, order_date);
					while (!isValidDate(order_date)) {
						cout << "Invalid date" << endl;
						cout << "Please input order date in format 'YYYY-MM-DD': ";
						getline(cin, order_date);
					}
					cout << "Enter the amount of ordered servings: ";
					cin >> quantity;
					while (quantity < 0) {
						cout << "Error! Cost cannot be negative" << endl;
						cout << "Please enter the amount of ordered servings: ";
						cin >> quantity;
					}

					Order newOrder(dish_id, order_date, quantity);
					newOrder.addOrder(conn);
					ostringstream ss;
					ss << "Added new order with ID = " << newOrder.getId() << " ";
					logAction(outFile, ss.str());
					break;
				}
				case (4): {
					Dish::showDishes(conn);
					logAction(outFile, "Analytical request");
					break;
				}
				case (5): {
					Order::showOrders(conn);
					logAction(outFile, "Analytical request");
					break;
				}
				case (6): {
					Category::totalRevenue(conn);
					logAction(outFile, "Analytical request");
					break;
				}

				case (7): {
					Dish::top3Dish(conn);
					logAction(outFile, "Analytical request");
					break;
				}

				case (8): {
					Order::averageCost(conn);
					logAction(outFile, "Analytical request");
					break;
				}

				case (9): {
					Order::countOrders(conn);
					logAction(outFile, "Analytical request");
					break;
				}

				case (10): {
					cout << "Exiting programme...";
					conn.close();
					break;
				}
				default: {
					cout << "Error! Invalid choice" << endl;
				}
				}

			} while (choice != 10);

		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			return 1;
		}
		outFile.close();
	}

	return 0;
}
