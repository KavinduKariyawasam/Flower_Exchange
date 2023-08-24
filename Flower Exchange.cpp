#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

enum class Side {Buy, Sell};

///////////////////// Creating the structure of the order (input file)/////////////////////////////
class Order {
public:
	string Client_Order_ID;
	string Instrument;
	int side;
	int Quantity;
	double Price;
};


///////////////////////// Order book (BUY) ///////////////////////

class Order_Book {
public:
	string id;
	int qty;
	int price;
};

////////////////////////// Creating structure of the execution report /////////////////////////

class Execution {
public:
	string client_Or_ID;
	string order_ID;
	string instrument;
	int side;
	double price;
	int quantity;
	string status;
	string reason;
	//string time;
};


class Flower_Trading {
public:
	vector<string> instrument_list = { "Rose", "Lavender", "Lotus", "Tulip", "Orchid" };
    vector<Order*> orders;      // Vector to store order details
	vector<Execution*> report;   // Vector to store final report details

	///////////////////// function to load the order csv file //////////////////////////////
	void Read_CSV(const string& csv_path) {
		ifstream file(csv_path);

		if (!file.is_open()) {
			cout << "Failed to open the file" << endl;
			return;
		}

		string line, field;

		getline(file, line);     // Neglecting the first line which contains headers
		
		// Creating order object for each order
		while (getline(file, line)) {
			istringstream iss(line);
			Order *order = new Order;

			getline(iss, order->Client_Order_ID, ',');
			getline(iss, order->Instrument, ',');

			getline(iss, field, ',');
			order->side = stoi(field);

			getline(iss, field, ',');
			order->Quantity = stoi(field);

			getline(iss, field, ',');
			order->Price = stod(field);
			
			orders.push_back(order);
		}

		file.close();

		// Print the orders stored in the vector
		/*
		for (const Order* order : orders) {
			cout << "Client Order ID: " << order->Client_Order_ID << " | Instrument: " << order->Instrument
				<< " | Side: " << static_cast<int>(order->side) << " | Quantity: " << order->Quantity
				<< " | Price: " << order->Price << endl;
		}	
		*/
	}  

	///////////// Checking the validity of the order /////////////////
	bool is_Valid_Order(const Order* order, Execution* transaction) {
		if (!std::count(instrument_list.begin(), instrument_list.end(), order->Instrument)) {
			transaction->status = "Rejected";
			transaction->reason = "Instrument is not supported";
			return false;
		}
		else if (order->Price < 0) {
			transaction->status = "Rejected";
			transaction->reason = "Invalid price";
			return false;
		}
		else if (order->Quantity < 10 || order->Quantity > 1000) {
			transaction->status = "Rejected";
			transaction->reason = "Quantity is out of the range.";
			return false;
		}
		else if (order->Quantity % 10 != 0) {
			transaction->status = "Rejected";
			transaction->reason = "Quantity must be a multiple of 10.";
			return false;
		}
		else {
			transaction->status = "New";
			transaction->reason = "";
			return true;
		}
	}

	////////////////////////Processing the order ///////////////////////////////////
	void Process_Order() {
		vector<Order_Book*> Buy_Rose;
		vector<Order_Book*> Sell_Rose;

		int i = 1;      // For unique order number

		auto ascending = [](Order_Book* a, Order_Book* b) {
			return a->price < b->price;
		};

		auto descending = [](Order_Book* a, Order_Book* b) {
			return a->price < b->price;
		};

		for (const Order* order : orders) {
			Execution* transaction = new Execution;

			Order_Book* Lavender_b = new Order_Book;
			Order_Book* Lotus_b = new Order_Book;
			Order_Book* Tulip_b = new Order_Book;
			Order_Book* Orchid_b = new Order_Book;

			Order_Book* Lavender_s = new Order_Book;
			Order_Book* Lotus_s = new Order_Book;
			Order_Book* Tulip_s = new Order_Book;
			Order_Book* Orchid_s = new Order_Book;

			bool x = is_Valid_Order(order, transaction);		// Checking the validity

			if (order->Instrument == "Rose") {
				Order_Book* Rose_b = new Order_Book;
				Order_Book* Rose_s = new Order_Book;
				if (order->side == 1) {
					Rose_b->id = order->Client_Order_ID;
					Rose_b->qty = order->Quantity;
					Rose_b->price = order->Price;
				}
				else {
					Rose_s->id = order->Client_Order_ID;
					Rose_s->qty = order->Quantity;
					Rose_s->price = order->Price;
				}
				Buy_Rose.push_back(Rose_b);
				Sell_Rose.push_back(Rose_s);
				sort(Buy_Rose.begin(), Buy_Rose.end(), ascending);
				sort(Sell_Rose.begin(), Sell_Rose.end(), descending);
				
							


				// To check the report (temporaty)
				transaction->client_Or_ID = order->Client_Order_ID;
				transaction->order_ID = "ord" + to_string(i);    // Generating a unique order ID
				transaction->instrument = order->Instrument;
				transaction->price = order->Price;
				transaction->quantity = order->Quantity;
			}

			report.push_back(transaction);
			i++;
		}


		for (const Order_Book* j : Buy_Rose) {
			cout << "Order ID: " << j->id << " | Quantity: " << j->qty
				<< " | Price: " << j->price << endl;
		}

		cout << "////////////////////////////////////////" << endl;

		for (const Execution*  j: report) {
			cout << "Client Order ID: " << j->client_Or_ID << "| Order ID: " << j->order_ID << " | Instrument: " << j->instrument
				<< " | Quantity: " << j->quantity
				<< " | Price: " << j->price << " | Status: " << j->status << " | Reason: " << j->reason << endl;
		}
	}
};


///////////////////////////////// Main Function /////////////////////////////////////////
int main() {
	Flower_Trading Trading_app;
	string File_Path = "D:\\University\\LSEG\\Flower Exchange\\ex2.csv" ;
	Trading_app.Read_CSV(File_Path);
	Trading_app.Process_Order();

	return 0;
}
