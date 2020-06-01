#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <mysql.h>
#include <algorithm>
#include <vector>
#include <map>
#include <iomanip>
#include "TextTable.h"

using namespace std;

bool is_admin = false;
int attempt_login = 3;
string temp;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
vector<map<string, string>> cart;

class db_response
{
public:
	static void ConnectionFunction()
	{
		// MYSQL SETTINGS
		const char* MYSQL_HOST = "localhost";
		const char* MYSQL_USER = "root";
		const char* MYSQL_PASSWORD = "password";
		const char* MYSQL_DB = "kiosk";
		int MYSQL_HOST_PORT = 3306;

		conn = mysql_init(0);
		if (conn)
		{
			cout << "Database Connected" << endl;
			cout << "Press any key to continue..." << endl;
			//getch();
			system("cls");
		}
		else
			cout << "Failed To Connect!" << mysql_errno(conn) << endl;
		conn = mysql_real_connect(conn, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB, MYSQL_HOST_PORT, NULL, 0);
		if (conn)
		{
			cout << "Database Connected To MySql" << conn << endl;
			cout << "Press any key to continue..." << endl;
			//getch();
			system("cls");
		}
		else
			cout << "Failed To Connect!" << mysql_errno(conn) << endl;
	}
};

// UI
void headerLogo();
void menuCustomer();

// Function for client
vector<map<string, string>> findAllCategories();
vector<map<string, string>> findItemByCategory(string);
void printItemCategories(vector<map<string, string>>);
void printItems(vector<map<string, string>>, map<string, string>);
void viewItemCategories(string);
void viewItemsByCategory(map<string, string>);
void viewAllOrders(string, string);
void viewAllOrderItems(string, string);
void viewCart();
void removeFromCart(string);
void itemCheckout();
void printReceipt(string);

// Function for Staff
//// UI
void menuStaff(string, string);

//// Staff Management
void viewStaff(string, string);
void updateStaff(string, string, vector<map<string, string>>, string);
void addStaff(string, string);
void deleteStaff(string, string);
void login(string);
void logout(string);

//// Inventory Management
void updateItems(string, string);
void updateCategoryDiscount(string, string);

//// Sales analysis
void viewMonthlySale(string);

// Calculation Function
float calculateDiscountedPrice(float, float);

// Unknown
void endMenuStaff(string);

// Password hashing function
unsigned int SHF(string);
string ToHex(unsigned int);

int main()
{
	system("cls");

	is_admin = false;
	attempt_login = 3;

	system("SELF ORDERING SYSTEM");
	system("color 0f");
	int role;

	db_response::ConnectionFunction();

	headerLogo();

	cout << "Please choose:" << endl;
	cout << "1. Guest (For Customer)" << endl;
	cout << "2. Login (For Staff)" << endl << "Selection :";
	getline(cin, temp);
	stringstream (temp) >> role; 

	if (role == 1) 
	{
		menuCustomer();
	}
	else if (role == 2)
	{
		login("");
	}
	else
	{
		main();
	}
}

void login(string error)
{
	string username, password;

	headerLogo();

	if (error != "")
	{
		cout << error << endl;
	}

	cout << "Notes : " << attempt_login << " attempt(s) left" << endl;

	cout << "Please enter username: ";
	getline(cin, username);

	cout << "Password: ";
	getline(cin, password);

	string findbyid_query = "SELECT is_admin, name FROM staff WHERE username = '" + username + "' AND password='" + password + "'";
	const char* _q = findbyid_query.c_str();
	mysql_query(conn, _q);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);

	if (row) 
	{
		int admin;
		stringstream(row[0]) >> admin;
		if (admin == 1)
		{
			is_admin = true;
		}
		menuStaff(row[1], "");
	}
	else
	{
		if (attempt_login == 1)
		{
			attempt_login = 3;
			main();
		}
		else
		{
			attempt_login--;
			login("Incorrect Username/Password!");
		}
	}
}

void menuCustomer() {

	int chooseOneFromMenu = 0;
	char exitSurity;

	headerLogo();

	cout << "Please Select Menu" << endl;
	cout << "1. See available items" << endl;
	cout << "2. View cart" << endl;
	cout << "3. Back" << endl;

	cout << endl;
	cout << "Selection :";
	getline(cin, temp);
	stringstream (temp) >> chooseOneFromMenu; 

	switch (chooseOneFromMenu)
	{
		case 1:
		{
			viewItemCategories("");
			break;
		}
		case 2:
		{
			viewCart();
			break;
		}
		case 3:
		{
			main();
			break;
		}
		case 9:
		{
		ExitProgram:
			cout << "Program terminating. Are you sure? (y/N): ";
			getline(cin, temp);
			stringstream (temp) >> exitSurity;
			if (exitSurity == 'y' || exitSurity == 'Y') {

			}
			else if (exitSurity == 'n' || exitSurity == 'N') {
				system("cls");
				menuCustomer();
			}
			else {
				cout << "Next time choose after read the corresponding line." << endl;
				goto ExitProgram;
			}
			break;
		}
		default:
		{
			cout << "Please choose between 1 - 2. Press Enter To Continue...";
			_getch();
			system("cls");
			menuCustomer();
			break;
		}
	}

}

void menuStaff(string name, string error) 
{

	string menu;
	headerLogo();

	cout << "Hye! " << name << "," << endl << endl;
	cout << "-------------------------------" << endl;
	cout << "            ADMIN PANEL        " << endl;
	cout << "-------------------------------" << endl;
	if (error != "")
	{
		cout << error << endl;
	}
	cout << "1. View All Orders" << endl;

	if (is_admin) {
		cout << "2. Insert/Update Items" << endl;
		cout << "3. View Monthly Sale" << endl;
		cout << "4. View All Staff" << endl;
		cout << "5. Add Staff" << endl;
		cout << "6. Update Category Discount" << endl;
	}

	cout << "Logout? (X/x)" << endl;

	cout << endl;
	cout << "Selection :";
	getline(cin, menu);

	if(menu == "1")
	{
		viewAllOrders(name, "");
	}
	else if (menu == "2" && is_admin)
	{
		updateItems(name, "");
	}
	else if (menu == "3" && is_admin)
	{
		viewMonthlySale(name);
	}
	else if (menu == "4" && is_admin)
	{
		viewStaff(name, "");
	}
	else if (menu == "5" && is_admin)
	{
		addStaff(name, "");
	}
	else if (menu == "6" && is_admin)
	{
		updateCategoryDiscount(name, "");
	}
	else if (menu == "X" || menu == "x")
	{
		string sure;
		cout << "Are you sure to logout?(Y/y)";
		getline(cin, sure);
		if (sure == "Y" || sure == "y")
		{
			main();
		}
		else
		{
			menuStaff(name, "");
		}
	}
	else
	{
		menuStaff(name, "Please select correct key!");
	}
}

vector<map<string, string>> findAllCategories()
{
	map<string, string> category;
	vector<map<string, string>> categories;

	mysql_query(conn, "SELECT id, name, code, discount, is_discount FROM categories ORDER BY id ASC");
	res = mysql_store_result(conn);

	while (row = mysql_fetch_row(res))
	{
		category["id"] = row[0];
		category["name"] = row[1];
		category["code"] = row[2];
		category["discount"] = row[3];
		category["is_discount"] = row[4];
		categories.push_back(category);
	}
	return categories;
}

void printItemCategories(vector<map<string, string>> categories)
{
	cout << " >>    SELECT PRODUCT CATEGORIES     <<" << endl << endl;
	for (int i = 0; i < categories.size(); i++)
	{
		cout << categories[i]["id"] << ". " << categories[i]["name"] << " ";
		if (categories[i]["is_discount"] == "1")
		{
			cout << "(ON " << categories[i]["discount"] << "% DISCOUNT NOW!!)";
		}
		cout << endl;
	}
}

void viewItemCategories(string error)
{
	system("cls");

	headerLogo();

	if (error != "")
	{
		cout << error << endl << endl;
	}

	string choice;
	
	vector<map<string, string>> categories = findAllCategories();

	printItemCategories(categories);
	
	cout << endl << "Enter (M/m) to go to Main Menu" << endl;
	cout << endl << "Selection : ";
	getline(cin, choice);

	if (choice == "M" || choice == "m")
	{
		menuCustomer();
	}

	for (int i = 0; i < categories.size(); i++)
	{
		if (choice == categories[i]["id"])
		{
			viewItemsByCategory(categories[i]);
			break;
		}
	}
	viewItemCategories("Please Enter Correct ID");
}

vector<map<string, string>> findItemByCategory(string category_id)
{
	map<string, string> item;
	vector<map<string, string>> items;

	string finditem_query = "SELECT id, code, name, price, stock FROM items WHERE category='" + category_id + "' ORDER BY code ASC";
	const char* qi = finditem_query.c_str();
	mysql_query(conn, qi);
	res = mysql_store_result(conn);

	while (row = mysql_fetch_row(res))
	{
		item["id"] = row[0];
		item["code"] = row[1];
		item["name"] = row[2];
		item["price"] = row[3];
		item["stock"] = row[4];
		items.push_back(item);
	}
	return items;
}

void printItems(vector<map<string, string>> items, map<string, string> category)
{
	int is_discount;
	float discount;

	stringstream(category["discount"]) >> discount;
	stringstream(category["is_discount"]) >> is_discount;

	TextTable t('-', '|', '+');

	t.add("CODE");
	t.add("PRICE (RM)");
	t.add("NAME");
	t.add("STOCK");
	if (is_discount == 1)
	{
		t.add("DISCOUNTED PRICE");
	}
	t.endOfRow();

	for (int i = 0; i < items.size(); i++)
	{
		t.add(items[i]["code"]);
		t.add(items[i]["price"]);
		t.add(items[i]["name"]);
		int stock;
		stringstream(items[i]["stock"]) >> stock;

		if (stock > 0) {
			t.add(items[i]["stock"]);
		}
		else {
			t.add("OUT OF STOCK");
		}

		if (is_discount == 1)
		{
			float price;
			string discounted_price;
			stringstream(items[i]["price"]) >> price;
			stringstream ss;
			ss << fixed << setprecision(2) << calculateDiscountedPrice(price, discount);
			ss >> discounted_price;
			t.add(discounted_price);
		}

		t.endOfRow();
	}

	t.setAlignment(5, TextTable::Alignment::RIGHT);

	cout << t;
}

void viewItemsByCategory(map<string, string> category)
{
	system("cls");

	headerLogo();

	int is_discount;
	float discount;

	stringstream(category["discount"]) >> discount;
	stringstream(category["is_discount"]) >> is_discount;
	vector<map<string, string>> items = findItemByCategory(category["id"]);

	headerLogo();

	cout << " >>    PICK YOUR PRODUCTS     <<" << endl << endl;

	printItems(items, category);

	cout << endl;

	string again;
	string item_error = "";
	bool proceed = false;

	do
	{
		again = "";
		string code;

		if (item_error != "") {
			cout << item_error << endl;
			item_error = "";
		}

		cout << "Insert Item Code (Item No. " << cart.size() << ')' << endl;
		cout << "CODE :";
		getline(cin, code);
		
		for (int i = 0; i < items.size(); i++)
		{
			int stock;
			float price;
			stringstream(items[i]["stock"]) >> stock;
			stringstream(items[i]["price"]) >> price;
			if (code == items[i]["code"])
			{
				float amount;
				cout << "AMOUNT :";
				getline(cin, temp);
				stringstream(temp) >> amount;
				if (amount > 0 && amount <= stock)
				{
					map<string, string> bought_item;
					float calc;
					bought_item = items[i];
					stringstream(amount) >> temp;
					bought_item["amount"] = temp;
					calc = amount * price;
					stringstream ss;
					ss << fixed << setprecision(2) << calc;
					ss >> bought_item["total_price"];
					
					if (is_discount == 1) 
					{
						stringstream ss;
						ss << fixed << setprecision(2) << calculateDiscountedPrice(amount * price, discount);
						ss >> bought_item["discounted_price"];
					}
					else
					{
						bought_item["discounted_price"] = bought_item["total_price"];
					}

					cart.push_back(bought_item);
					stringstream(stock - amount) >> items[i]["stock"];
					proceed = true;
					break;
				}
				else if (amount > stock)
				{
					item_error = "Not enough stock!";
					break;
				}
				else
				{
					item_error = "Please enter the right amount";
					break;
				}
			}
		}

		if (proceed && item_error == "")
		{
			cout << "View cart (C/c)" << endl;
			cout << "Add other item (Y/y)" << endl;
			cout << "Choose other category (B/b)" << endl;
			cout << "Cancel order (X/x)" << endl;
			cout << "Press any other key to checkout.." << endl;
			getline(cin, temp);
			stringstream(temp) >> again;

			if (again == "X" || again == "x") 
			{
				cart.clear();
				menuCustomer();
			}
			else if (again == "B" || again == "b") 
			{
				viewItemCategories("");
			}
			else if (again == "C" || again == "c") 
			{
				viewCart();
			}
		}
		else if (item_error == "")
		{
			item_error = "Item Code: " + code + " does not exist";
		}

	} while ((again == "Y") || (again == "y") || !proceed);
	itemCheckout();
}

void itemCheckout()
{
	if (cart.size() > 0)
	{
		map<string, string> orders;
		float total_price = 0;
		float after_discount = 0;

		for (int i = 0; i < cart.size(); i++)
		{
			float price;
			stringstream(cart[i]["total_price"]) >> price;
			total_price += price;
			stringstream(cart[i]["discounted_price"]) >> price;
			after_discount += price;
		}

		stringstream tp;
		tp << fixed << setprecision(2) << total_price;
		tp >> orders["total_price"];
		stringstream ad;
		ad << fixed << setprecision(2) << after_discount;
		ad >> orders["after_discount"];

		string insert_order_q = "INSERT INTO orders (total_price, after_discount) VALUE ('" + orders["total_price"] + "', '" + orders["after_discount"] + "')";
		const char* lid_str = insert_order_q.c_str();
		mysql_query(conn, lid_str);

		string get_order_id = "SELECT id FROM orders ORDER by id DESC LIMIT 1";
		lid_str = get_order_id.c_str();
		
		if (!mysql_query(conn, lid_str)) {
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);

			orders["id"] = row[0];

			for (int i = 0; i < cart.size(); i++)
			{
				string order_q = "INSERT INTO order_items (orders, items, amount, total_price, after_discount) VALUES ('" + orders["id"] + "', '" + cart[i]["id"] + "','" + cart[i]["amount"] + "','" + cart[i]["total_price"] + "','" + cart[i]["discounted_price"] + "')";
				const char* order_str = order_q.c_str();
				mysql_query(conn, order_str);

				string update_stock_q = "UPDATE items SET stock=(stock-" + cart[i]["amount"] + ") WHERE id='" + cart[i]["id"] + "'";
				const char* usq = update_stock_q.c_str();
				mysql_query(conn, usq);
			}
		}
		else {
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
		cart.clear();
		printReceipt(orders["id"]);
		cout << endl << "Press any key to proceed...";
		getline(cin, temp);
		menuCustomer();
	}
}

float calculateDiscountedPrice(float price, float discount)
{
	float discounted_price;
	string new_price;
	discounted_price = price - (discount / (float)100 * price);
	return discounted_price;
}

void viewCart() {

	system("cls");

	string checkout;
	float item_total, total_overall = 0;

	headerLogo();

	cout << " >>    YOUR CART     <<" << endl << endl;

	if (cart.size() > 0)
	{
		TextTable t('-', '|', '+');

		t.add("ID");
		t.add("ITEM CODE");
		t.add("ITEM");
		t.add("PRICE PER ITEM");
		t.add("AMOUNT");
		t.add("TOTAL PRICE");
		t.add("PRICE AFTER DISCOUNT");
		t.endOfRow();

		for (int i = 0; i < cart.size(); i++)
		{
			string num;
			stringstream ss;
			int j = i + 1;
			ss << fixed << j;
			ss >> num;
			stringstream(j) >> num;
			t.add(num);
			t.add(cart[i]["code"]);
			t.add(cart[i]["name"]);
			t.add(cart[i]["price"]);
			t.add(cart[i]["amount"]);
			t.add(cart[i]["total_price"]);
			t.add(cart[i]["discounted_price"]);
			t.endOfRow();
			stringstream(cart[i]["discounted_price"]) >> item_total;
			total_overall += item_total;
		}
		
		string total;
		stringstream ss;
		ss << fixed << setprecision(2) << total_overall;
		ss >> total;

		cout << t << endl;
		cout << "Total: RM" + total << endl << endl;
		cout << "Add item?(Y/y) | ";
		cout << "Checkout?(C/c) | ";
		cout << "Cart?(X/x)" << endl;
		cout << "Remove item form cart? Enter ID: " << endl << endl;
	}
	else
	{
		cout << " >>    YOUR CART IS EMPTY     <<" << endl << endl;
		cout << "Buy some item?(Y/y)" << endl;
	}

	
	cout << "Selection: ";
	getline(cin, checkout);
	
	if (checkout == "Y" || checkout == "y")
	{
		viewItemCategories("");
	}
	else if ((checkout == "C" || checkout == "c") && cart.size() > 0)
	{
		itemCheckout();
	}
	else if (checkout == "X" || checkout == "x")
	{
		cart.clear();
		menuCustomer();
	}
	else 
	{
		removeFromCart(checkout);
	}
}

void removeFromCart(string id) {
	int num;
	stringstream(id) >> num;
	if (num > 0 && num <= cart.size())
	{
		cart.erase(cart.begin() + num - 1);
		viewCart();
	}
	else
	{
		menuCustomer();
	}
}

void printReceipt(string id) {
	system("cls");

	string date, total, total_discounted;

	string find_order = "SELECT items.code AS code, items.name AS name, items.price AS price, amount, order_items.total_price AS total_price,  order_items.after_discount AS after_discount, orders.total_price AS total, orders.after_discount AS total_discounted, orders.created_at AS created_at FROM order_items INNER JOIN items ON order_items.items=items.id INNER JOIN orders ON order_items.orders=orders.id WHERE orders='" + id + "';";
	const char* lid_str = find_order.c_str();
	if (mysql_query(conn, lid_str))
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	res = mysql_store_result(conn);

	TextTable t('-', '|', '+');

	t.add("CODE");
	t.add("ITEM");
	t.add("PRICE PER ITEM");
	t.add("AMOUNT");
	t.add("TOTAL PRICE");
	t.add("PRICE AFTER DISCOUNT");
	t.endOfRow();

	while (row = mysql_fetch_row(res)) {
		t.add(row[0]);
		t.add(row[1]);
		t.add(row[2]);
		t.add(row[3]);
		t.add(row[4]);
		t.add(row[5]);
		total = row[6];
		total_discounted = row[7];
		date = row[8];
		t.endOfRow();
	}

	t.setAlignment(6, TextTable::Alignment::RIGHT);

	headerLogo();
	cout << "========================================================================" << endl;
	cout << "                           RECEIPT #" + id<< endl << endl;
	cout << "                     Time / Date : " << date << endl;
	cout << "========================================================================" << endl;
	
	cout << t;

	cout << endl;

	cout << endl << endl << "Total                                                       :RM " << total;
	cout << endl << "========================================================================" << endl;
	cout << "Total After Discount                                        :RM " << total_discounted;
	cout << endl << "========================================================================" << endl << endl;

	cout << "========================================================================" << endl;
	cout << "                     THANK YOU, PLEASE COME AGAIN" << endl;
	cout << "========================================================================" << endl;
	
}




//STAFF FUNCTION

void viewAllOrders(string auth_name, string error = "") {
	system("cls");
	string id;
	
	mysql_query(conn, "SELECT id, total_price, after_discount, created_at FROM orders");
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);

	headerLogo();

	if (error != "")
	{
		cout << error << endl << endl;
	}

	cout << " >>    ALL ORDERS     <<" << endl << endl;

	if(row) {

		TextTable t('-', '|', '+');

		t.add("ID");
		t.add("TOTAL PRICE (RM)");
		t.add("DISCOUNTED PRICE (RM)");
		t.add("ORDERED DATE/TIME");
		t.endOfRow();

		do {
			t.add(row[0]);
			t.add(row[1]);
			t.add(row[2]);
			t.add(row[3]);
			t.endOfRow();
			row = mysql_fetch_row(res);
		} while (row);
		
		t.setAlignment(4, TextTable::Alignment::RIGHT);

		cout << t << endl;

		cout << "View order? Enter order ID or (B/b) to go back" << endl;
		
		getline(cin, id);

		if (id == "B" || id == "b")
		{
			menuStaff(auth_name, "");
		}
		else
		{
			viewAllOrderItems(auth_name, id);
		}
	}
	else
	{
		cout << "No order found" << endl << endl;

		cout << "Enter any key to go back" << endl;
		getline(cin, id);
		menuStaff(auth_name, "");
	}
}

void viewAllOrderItems(string auth_name, string id)
{
	printReceipt(id);

	cout << "(B/b) to go back or any key to go to staff menu" << endl;

	string select;
	getline(cin, select);

	if (select == "B" || select == "b")
	{
		viewAllOrders(auth_name, "");
	}
	else
	{
		menuStaff(auth_name, "");
	}
}

void updateItems(string auth_name, string error)
{
	system("cls");
	
	string select;
	vector<map<string, string>> categories = findAllCategories();

	printItemCategories(categories);

	if (error != "")
	{
		cout << error << endl;
	}
	cout << "Selection: ";
	getline(cin, select);

	for (int i = 0; i < categories.size(); i++)
	{
		if (select == categories[i]["id"])
		{
			vector<map<string, string>> items = findItemByCategory(categories[i]["id"]);
			system("cls");
			cout << " >>    LIST OF ITEMS FOR CATEGORY: "<< categories[i]["name"] << "     <<" << endl << endl;
			printItems(items, categories[i]);
			cout << "";
			getline(cin, select);
		}
	}

	updateItems(auth_name, "Please select category ID");
}

void viewMonthlySale(string auth_name) {

	//max day in a year
	int year,
		month = 1;

	headerLogo();

	cout << ">>  Monthly Sale Report <<" << endl << endl;


	do {
		cout << "Please Enter Years(EX:2020). Year > :";
		getline(cin, temp);
		stringstream (temp) >> year;

	} while (year < 1990 || year > 2020);

	string date_str[31];
	float monthly_sale[12] = { 0.00 };

	while (month <= 12) {

		string dt = to_string(year) + "-" + to_string(month);


		string daily_sale_str = "SELECT created_at, sum(after_discount) from orders where YEAR(created_at) = '" + to_string(year) + "' AND MONTH(created_at) = '" + to_string(month) + "' group by month(created_at);";
		const char* daily_sale_q = daily_sale_str.c_str();
		mysql_query(conn, daily_sale_q);
		res = mysql_store_result(conn);

		int ttlRow = mysql_num_rows(res);

		if (ttlRow > 0) {
			row = mysql_fetch_row(res);
			monthly_sale[month] = stof(row[1]);
		}
		else {
			monthly_sale[month] = stof("0");
		}

		date_str[month] = dt;
		month++;

	}

	//reuse var
	month = 1;

	headerLogo();
	cout << "MONTHLY SALE REPORT FOR " + to_string(year) << endl << endl;

	TextTable t('-', '|', '+');

	t.add("DATE");
	t.add("TOTAL SALE");
	t.endOfRow();

	while (month <= 12) {

		t.add(date_str[month]);
		t.add("RM " + to_string(monthly_sale[month]));
		t.endOfRow();
		month++;
	}

	t.setAlignment(2, TextTable::Alignment::RIGHT);
	cout << t;

	endMenuStaff(auth_name);
}

void addStaff(string auth_name, string error) 
{
	system("cls");

	string username, password, name, admin;

	headerLogo();
	cout << ">>        Add Staff      <<" << endl;
	if (error != "")
	{
		cout << error << endl;
	}
	cout << "Enter new username: ";
	getline(cin, username);
	cout << "Enter new password: ";
	getline(cin, password);
	cout << "Enter new name: ";
	getline(cin, name);
	cout << "Is this staff an admin?(Y/y)";
	getline(cin, admin);
	if (admin == "Y" || admin == "y")
	{
		admin = "1";
	}
	else
	{
		admin = "0";
	}
	const char* _u = ("INSERT INTO staff (username, password, name, is_admin) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + admin + "')").c_str();
	
	if (!mysql_query(conn, _u))
	{
		viewStaff(auth_name, "");
	}
	else
	{
		addStaff(auth_name, "Username already exist! Please pick other username.");
	}
}

void deleteStaff(string auth_name, string id)
{
	const char* _q = ("DELETE FROM staff WHERE id='" + id + "' LIMIT 1").c_str();
	mysql_query(conn, _q);
	string error;
	error = "Staff with ID " + id + " has been deleted!";
	viewStaff(auth_name, "");
}

void viewStaff(string auth_name, string error) 
{
	system("cls");

	vector<map<string, string>> staffs;
	string staff_id;

	headerLogo();
	cout << ">>        Staff List      <<" << endl;

	mysql_query(conn, "SELECT id, username, name, is_admin FROM staff");
	res = mysql_store_result(conn);

	TextTable t('-', '|', '+');

	t.add("STAFF NO");
	t.add("USERNAME");
	t.add("NAME");
	t.add("ADMIN");
	t.endOfRow();

	while (row = mysql_fetch_row(res)) 
	{
		map<string, string> staff;
		staff["id"] = row[0];
		staff["username"] = row[1];
		staff["name"] = row[2];
		staff["is_admin"] = row[3];
		staffs.push_back(staff);
		t.add(row[0]);
		t.add(row[1]);
		t.add(row[2]);

		int is_admin;
		stringstream (row[3]) >> is_admin; 

		if (is_admin == 1) {
			t.add("YES");
		}
		else {
			t.add("NO");
		}

		t.endOfRow();
	}

	t.setAlignment(4, TextTable::Alignment::RIGHT);

	cout << t;

	if (error != "")
	{
		cout << error << endl;
	}

	cout << "Enter Staff No: (Enter 0 to exit) : => ";
	getline(cin, staff_id);
	
	if (staff_id == "0") {
		menuStaff(auth_name, "");
	}

	for (int i = 0; i < staffs.size(); i++)
	{
		if (staff_id == staffs[i]["id"])
		{
			string action;
			cout << "Edit (E/e) or Delete (X/x) Staff? Press other key to cancel" << endl;
			getline(cin, action);
			if (action == "E" || action == "e")
			{
				updateStaff(auth_name, staff_id, staffs, "");
			}
			else if (action == "X" || action == "x")
			{
				cout << "Are you sure?(Y/y) :";
				getline(cin, action);
				if (action == "Y" || action == "y")
				{
					deleteStaff(auth_name, staff_id);
				}
				else
				{
					viewStaff(auth_name, "");
				}
			}
			else
			{
				viewStaff(auth_name, "");
			}
		}
	}
	viewStaff(auth_name, "User does not exist! Please enter correct ID");
}

void updateStaff(string auth_name, string id, vector<map<string, string>> staffs, string error)
{
	string username, password, name;
	if (error != "")
	{
		cout << error << endl;
	}
	cout << "Enter new username: ";
	getline(cin, username);
	cout << "Enter new password: ";
	getline(cin, password);
	cout << "Enter new name: ";
	getline(cin, name);

	for (int i = 0; i < staffs.size(); i++)
	{
		if (username == staffs[i]["username"] && id != staffs[i]["id"])
		{
			updateStaff(auth_name, id, staffs, "Username already existed! Please choose other username");
		}
	}

	const char* _u = ("UPDATE staff SET username='" + username + "', password='" + password + "', name='" + name + "' WHERE id='" + id + "'").c_str();
	mysql_query(conn, _u);
	viewStaff(auth_name, "");
}

void updateCategoryDiscount(string auth_name, string error = "") {

	headerLogo();

	if (error != "")
	{
		cout << error << endl << endl;
	}

	map<string, string> category;
	vector<map<string, string>> categories;

	mysql_query(conn, "SELECT name, code, discount, is_discount FROM categories");
	res = mysql_store_result(conn);

	while (row = mysql_fetch_row(res))
	{
		category["name"] = row[0];
		category["code"] = row[1];
		category["discount"] = row[2];
		category["is_discount"] = row[3];
		categories.push_back(category);
	}

	TextTable t('-', '|', '+');

	t.add("CODE");
	t.add("CATEGORY NAME");
	t.add("DISCOUNT RATE(%)");
	t.add("ACTIVE");
	t.endOfRow();

	for (int i = 0; i < categories.size(); i++)
	{
		int active;
		stringstream(categories[i]["is_discount"]) >> active;
		t.add(categories[i]["code"]);
		t.add(categories[i]["name"]);
		t.add(categories[i]["discount"]);
		if (active) {
			t.add("ACTIVE");
		}
		else {
			t.add("INACTIVE");
		}
		t.endOfRow();
	}

	t.setAlignment(4, TextTable::Alignment::RIGHT);

	cout << t << endl;

	string selection;
	cout << "Enter (Y/y) to update or other key to back to Menu. : => ";
	getline(cin, selection);

	if (selection == "Y" || selection == "y") 
	{
		string choice;
		cout << "Select category to update:" << endl;
		cout << endl << "Selection : ";
		getline(cin, choice);
		for (int i = 0; i < categories.size(); i++)
		{
			if (choice == categories[i]["code"])
			{
				// UPDATE
				int active, new_discount;
				string reverse_is_discount;
				stringstream ss;
				stringstream(categories[i]["is_discount"]) >> active;
				if (active)
				{
					cout << "Enter new discount for " << categories[i]["name"] << "? Default is 0" << endl;
					cout << "Discount(%): ";
					getline(cin, temp);
					stringstream(temp) >> new_discount;
					ss << fixed << new_discount;
					ss >> temp;
					cout << "Deactivate discount for " << categories[i]["name"] << "?(Y/y) ";
					getline(cin, reverse_is_discount);
				}
				else
				{
					cout << "Enter new discount for " << categories[i]["name"] << "? Default is 0" << endl;
					cout << "Discount(%): ";
					getline(cin, temp);
					stringstream(temp) >> new_discount;
					ss << fixed << new_discount;
					ss >> temp;
					cout << "Activate discount for " << categories[i]["name"] << "??(Y/y) ";
					getline(cin, reverse_is_discount);
				}

				if(reverse_is_discount == "Y" || reverse_is_discount == "y")
				{
					string reverse_active;
					if (active)
					{
						reverse_active = "0";
					}
					else
					{
						reverse_active = "1";
					}
					const char* ucd = ("UPDATE categories SET discount='" + temp + "', is_discount='" + reverse_active + "' WHERE code='" + choice + "'").c_str();
					mysql_query(conn, ucd);
					updateCategoryDiscount(auth_name, "");
				}
				else
				{
					const char* ucd = ("UPDATE categories SET discount='" + temp + "' WHERE code='" + choice + "'").c_str();
					mysql_query(conn, ucd);
					updateCategoryDiscount(auth_name, "");
				}
			}
		}
		updateCategoryDiscount(auth_name, "Please enter correct category CODE!");
	}
	else 
	{
		menuStaff(auth_name, "");
	}
}

void endMenuStaff(string auth_name) {

	char choose;
ExitMenu:
	cout << "Press 'm' to Menu and any other key to Exit: ";
	getline(cin, temp);
	stringstream (temp) >> choose;
	 
	if (choose == 'm' || choose == 'M') {
		menuStaff(auth_name, "");
	}
	else {
		main();
	}
}

void headerLogo() {

	system("cls");
	cout << endl << "                  ===================================" << endl;
	cout << "                  || AWESOME PRODUCT FOR YOUR SKIN ||" << endl;
	cout << "                  ===================================" << endl << endl;
}

unsigned int SHF(string input)
{
	string Salt = "salt";
	unsigned int Salt_Byte;
	unsigned int Hash;
	for (int i = 0; i < input.length(); i++)
	{
		for (int j = 0; j < Salt.length(); j++)
		{
			Salt_Byte = Salt_Byte ^ (Salt[j]);
		}
		Hash = Hash ^ (input[i]);
		Hash = Hash * Salt_Byte;
	}
	return Hash;
}

string ToHex(unsigned int input)
{
	string HexHash;
	stringstream hexstream;
	hexstream << hex << input;
	HexHash = hexstream.str();
	transform(HexHash.begin(), HexHash.end(), HexHash.begin(), ::toupper);
	return HexHash;
}