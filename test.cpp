#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

enum class Side { Buy = 1, Sell = 2 };

///////////////////// Creating the structure of the order (input file)/////////////////////////////
class Order {
public:
	string Client_Order_ID;
	string Instrument;
	Side side;
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
	Side side;
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
	//vector<Execution*> report;   // Vector to store final report details

	void printExecutionReports() const {
		for (const Execution* report : executionReports) {
			std::cout << "Client Order ID: " << report->client_Or_ID << " | Order ID: " << report->order_ID
				<< " | Instrument: " << report->instrument << " | Side: "
				<< " | Price: " << report->price << " | Quantity: " << report->quantity
				<< " | Status: " << report->status << std::endl;
		}
	}

	///////////////////// function to load the order csv file //////////////////////////////
	vector<Order*> Read_CSV(const string& csv_path) {
		ifstream file(csv_path);

		if (!file.is_open()) {
			cout << "Failed to open the file" << endl;
			return orders;
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
			if (field == "Buy") {
				order->side = Side::Buy;
			}
			else if (field == "Sell") {
				order->side = Side::Sell;
			}

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
		return orders;
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
	//void Process_Order() {
	//	vector<Order_Book*> Buy_Rose;
	//	vector<Order_Book*> Sell_Rose;

	//	int i = 1;      // For unique order number

	//	auto ascending = [](Order_Book* a, Order_Book* b) {
	//		return a->price < b->price;
	//	};

	//	auto descending = [](Order_Book* a, Order_Book* b) {
	//		return a->price < b->price;
	//	};

	//	for (const Order* order : orders) {
	//		Execution* transaction = new Execution;

	//		Order_Book* Lavender_b = new Order_Book;
	//		Order_Book* Lotus_b = new Order_Book;
	//		Order_Book* Tulip_b = new Order_Book;
	//		Order_Book* Orchid_b = new Order_Book;

	//		Order_Book* Lavender_s = new Order_Book;
	//		Order_Book* Lotus_s = new Order_Book;
	//		Order_Book* Tulip_s = new Order_Book;
	//		Order_Book* Orchid_s = new Order_Book;

	//		bool x = is_Valid_Order(order, transaction);		// Checking the validity

	//		if (order->Instrument == "Rose") {
	//			Order_Book* Rose_b = new Order_Book;
	//			Order_Book* Rose_s = new Order_Book;
	//			if (order->side == 1) {
	//				Rose_b->id = order->Client_Order_ID;
	//				Rose_b->qty = order->Quantity;
	//				Rose_b->price = order->Price;
	//			}
	//			else {
	//				Rose_s->id = order->Client_Order_ID;
	//				Rose_s->qty = order->Quantity;
	//				Rose_s->price = order->Price;
	//			}
	//			Buy_Rose.push_back(Rose_b);
	//			Sell_Rose.push_back(Rose_s);
	//			sort(Buy_Rose.begin(), Buy_Rose.end(), ascending);
	//			sort(Sell_Rose.begin(), Sell_Rose.end(), descending);

	//			// To check the report (temporaty)
	//			transaction->client_Or_ID = order->Client_Order_ID;
	//			transaction->order_ID = "ord" + to_string(i);    // Generating a unique order ID
	//			transaction->instrument = order->Instrument;
	//			transaction->price = order->Price;
	//			transaction->quantity = order->Quantity;
	//		}

	//		report.push_back(transaction);
	//		i++;
	//	}


	//	for (const Order_Book* j : Buy_Rose) {
	//		cout << "Order ID: " << j->id << " | Quantity: " << j->qty
	//			<< " | Price: " << j->price << endl;
	//	}

	//	cout << "////////////////////////////////////////" << endl;

	//	for (const Execution* j : report) {
	//		cout << "Client Order ID: " << j->client_Or_ID << "| Order ID: " << j->order_ID << " | Instrument: " << j->instrument
	//			<< " | Quantity: " << j->quantity
	//			<< " | Price: " << j->price << " | Status: " << j->status << " | Reason: " << j->reason << endl;
	//	}
	//}
	


	void processOrders(const vector<Order*> orders) {
		for (const Order* order : orders) {
			processSingleOrder(order);
		}
	};

private:
	vector<Order_Book*> buyOrderBooks[5]; // One for each instrument
	vector<Order_Book*> sellOrderBooks[5]; // One for each instrument
	vector<Execution*> executionReports;

	void processSingleOrder(const Order* order) {
		vector<Order_Book*> orderBook;
		//= order->side == Side::Buy ? buyOrderBooks[getInstrumentIndex(order->Instrument)] : sellOrderBooks[getInstrumentIndex(order->Instrument)];
		if (order->side == Side::Buy) {
			orderBook = buyOrderBooks[getInstrumentIndex(order->Instrument)];
		}
		else if (order->side == Side::Sell) {
			orderBook = sellOrderBooks[getInstrumentIndex(order->Instrument)];
		}
		else {
			// Handle other cases or errors
		}
		int remainingQty = order->Quantity;

		//for (auto& bookOrder : orderBook) {
		for (auto bookOrderIt = orderBook.begin(); bookOrderIt != orderBook.end(); ++bookOrderIt) {
			auto& bookOrder = *bookOrderIt;
			if ((order->side == Side::Buy && order->Price >= bookOrder->price) ||
				(order->side == Side::Buy && order->Price <= bookOrder->price)) {

				int executedQty = std::min(remainingQty, bookOrder->qty);
				remainingQty -= executedQty;

				Execution* report = new Execution;
				report->client_Or_ID = order->Client_Order_ID;
				report->order_ID = generateOrderID(); // Implement a way to generate unique IDs
				report->instrument = order->Instrument;
				report->side = order->side;
				report->price = bookOrder->price;
				report->quantity = executedQty;

				if (remainingQty == 0) {
					report->status = "Filled";
					//orderBook.erase(bookOrder);
					bookOrderIt = orderBook.erase(bookOrderIt); // Erase the element and update iterator
					--bookOrderIt;
				}
				else {
					report->status = "PFill"; // Partially Filled
					bookOrder->qty -= executedQty;
				}

				executionReports.push_back(report);

				std::cout << "Client Order ID: " << report->client_Or_ID << " | Order ID: " << report->order_ID
					<< " | Instrument: " << report->instrument << " | Side: "
					<< " | Price: " << report->price << " | Quantity: " << report->quantity
					<< " | Status: " << report->status << std::endl;
			}


			if (remainingQty == 0) {
				break;
			}
		}

		//if (remainingQty > 0) {
		//	Order_Book* newBookOrder = new Order_Book;
		//	newBookOrder->qty = remainingQty;
		//	newBookOrder->price = order->Price;
		//	orderBook.push_back(newBookOrder);

		//	Execution* report = new Execution;
		//	report->client_Or_ID = order->Client_Order_ID;
		//	report->order_ID = generateOrderID(); // Implement a way to generate unique IDs
		//	report->instrument = order->Instrument;
		//	report->side = order->side;
		//	report->price = order->Price;
		//	report->quantity = remainingQty;
		//	report->status = "New";

		//	executionReports.push_back(report);
		//}
	}

	int getInstrumentIndex(const string& instrument) {
		// Assuming you have an instrument_list defined earlier
		auto it = std::find(instrument_list.begin(), instrument_list.end(), instrument);
		return (it != instrument_list.end()) ? std::distance(instrument_list.begin(), it) : -1;
	}

	string generateOrderID() {
		// Implement a way to generate unique order IDs
		return "OrderID";
	}

};


///////////////////////////////// Main Function /////////////////////////////////////////
int main() {
	vector<Order*> orders;
	Flower_Trading Trading_app;
	string File_Path = "D:\\University\\LSEG\\Flower Exchange\\ex2.csv" ;
	orders = Trading_app.Read_CSV(File_Path);
	Trading_app.processOrders(orders);
	Trading_app.printExecutionReports();
	return 0;
}
