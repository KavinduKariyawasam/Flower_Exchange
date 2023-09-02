#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

///////////////////// Creating the structure of the order (input file)/////////////////////////////
class Order {
public:
    string Client_Order_ID;
    string Instrument;
    string Ord_id;
    int side;
    int Quantity;
    double Price;
};

///////////////////////// Order book (BUY) ///////////////////////

class Order_Book {
public:
    string id;
    int qty;
    double price; // Changed to double
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
};

class Flower_Trading {
public:
    vector<string> instrument_list = { "Rose", "Lavender", "Lotus", "Tulip", "Orchid" };
    vector<Order*> orders;
    vector<Execution*> report;
    void Read_CSV(const string& csv_path) {
        ifstream file(csv_path);

        if (!file.is_open()) {
            cout << "Failed to open the file" << endl;
            return;
        }

        string line, field;

        getline(file, line);     // Neglecting the first line which contains headers
        int ord = 1;
        // Creating order object for each order
        while (getline(file, line)) {
            istringstream iss(line);
            Order* order = new Order;

            getline(iss, order->Client_Order_ID, ',');
            getline(iss, order->Instrument, ',');

            getline(iss, field, ',');
            order->side = stoi(field);

            getline(iss, field, ',');
            order->Quantity = stoi(field);

            getline(iss, field, ',');
            order->Price = stod(field);

            order->Ord_id = "ord" + to_string(ord);

            orders.push_back(order);
            ord++;

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

    // ... (other functions)
    bool is_Invalid_Order(const Order* order, Execution*& transaction) {
        if (!std::count(instrument_list.begin(), instrument_list.end(), order->Instrument)) {
            transaction->status = "Rejected";
            transaction->reason = "Instrument is not supported";
            return true;
        }
        else if (order->Price < 0) {
            transaction->status = "Rejected";
            transaction->reason = "Invalid price";
            return true;
        }
        else if (order->Quantity < 10 || order->Quantity > 1000) {
            transaction->status = "Rejected";
            transaction->reason = "Quantity is out of the range.";
            return true;
        }
        else if (order->Quantity % 10 != 0) {
            transaction->status = "Rejected";
            transaction->reason = "Quantity must be a multiple of 10.";
            return true;
        }
        return false;

    }




    void Process_Order() {
        vector<Order_Book*> Buy_Rose;
        vector<Order_Book*> Sell_Rose;

        vector<Order_Book*> Buy_Lavender;
        vector<Order_Book*> Sell_Lavender;

        vector<Order_Book*> Buy_Lotus;
        vector<Order_Book*> Sell_Lotus;

        vector<Order_Book*> Buy_Tulip;
        vector<Order_Book*> Sell_Tulip;

        vector<Order_Book*> Buy_Orchid;
        vector<Order_Book*> Sell_Orchid;

        int  i = 1;

        auto ascending = [](Order_Book* a, Order_Book* b) {
            return a->price < b->price;
        };

        auto descending = [](Order_Book* a, Order_Book* b) {
            return a->price > b->price;
        };

        for (Order* order : orders) {
            Execution* transaction = new Execution;
            transaction->client_Or_ID = order->Client_Order_ID;
            transaction->order_ID = order->Ord_id;
            transaction->instrument = order->Instrument;
            transaction->price = order->Price;
            transaction->quantity = order->Quantity;
            //cout << order->Client_Order_ID << endl;
            //bool x= is_Valid_Order(order, transaction);

            if (is_Invalid_Order(order, transaction)){
                report.push_back(transaction);
            }
            else {
                if (order->Instrument == "Rose") {
                    if (order->side == 1) { // Buy order
                        Order_Book* Rose_b = new Order_Book;
                        Rose_b->id = order->Client_Order_ID;
                        //cout << order->Client_Order_ID << endl;
                        Rose_b->qty = order->Quantity;
                        Rose_b->price = order->Price;
                        Buy_Rose.push_back(Rose_b);
                        sort(Buy_Rose.begin(), Buy_Rose.end(), ascending);
                    }
                    else { // Sell order
                        Order_Book* Rose_s = new Order_Book;
                        Rose_s->id = order->Client_Order_ID;
                        //cout << order->Client_Order_ID << endl;
                        Rose_s->qty = order->Quantity;
                        Rose_s->price = order->Price;
                        Sell_Rose.push_back(Rose_s);
                        sort(Sell_Rose.begin(), Sell_Rose.end(), descending);
                    }
                    if (order->side == 1) {
                        bool matched = false;

                        for (Order_Book* sell_order : Sell_Rose) {
                            //cout << order->Client_Order_ID << endl;
                            

                            if (order->Price >= sell_order->price) {
                                matched = true;

                                transaction->client_Or_ID = order->Client_Order_ID;

                                //cout << transaction->client_Or_ID << endl;
                                //transaction->order_ID = "ord" + to_string(i);
                                transaction->instrument = "Rose";  // Set the instrument correctly
                                transaction->price = order->Price; // Set the price correctly

                                if (order->Quantity == sell_order->qty) {
                                    transaction->status = "Fill";
                                    transaction->quantity = sell_order->qty; // Set the quantity correctly
                                    Sell_Rose.erase(Sell_Rose.begin());
                                    Buy_Rose.erase(Buy_Rose.begin());
                                    report.push_back(transaction);
                                    Execution* transaction = new Execution;
                                    transaction->status = "Fill";
                                    transaction->quantity = sell_order->qty;
                                    transaction->instrument = "Rose";  // Set the instrument correctly
                                    transaction->price = order->Price;
                                    transaction->client_Or_ID = sell_order->id;
                                    report.push_back(transaction);
                                    matched = true;
                                    break;
                                }
                                else if (order->Quantity < sell_order->qty) {
                                    transaction->status = "Fill";
                                    transaction->quantity = order->Quantity; // Set the quantity correctly
                                    sell_order->qty -= order->Quantity;
                                    Buy_Rose.erase(Buy_Rose.begin());
                                    report.push_back(transaction);

                                    Execution* sell_side = new Execution;
                                    sell_side->status = "Pfill";
                                    sell_side->quantity = order->Quantity;
                                    sell_side->instrument = "Rose";  // Set the instrument correctly
                                    sell_side->price = order->Price;
                                    sell_side->client_Or_ID = sell_order->id;
                                    report.push_back(sell_side);
                                    matched = true;
                                    break;
                                }
                                else { // buy_order->qty > sell_order->qty
                                    transaction->status = "PFill";
                                    transaction->quantity = sell_order->qty; // Set the quantity correctly
                                    order->Quantity -= sell_order->qty;
                                    report.push_back(transaction);

                                    Execution* transaction = new Execution;
                                    transaction->status = "Fill";
                                    transaction->quantity = sell_order->qty;
                                    transaction->instrument = "Rose";  // Set the instrument correctly
                                    transaction->price = order->Price;
                                    transaction->client_Or_ID = sell_order->id;
                                    Sell_Rose.erase(Sell_Rose.begin());
                                    report.push_back(transaction);
                                    matched = true;
                                    //break;

                                }
                            }
                            

                        }
                        if (!matched) {
                            // If no match occurred for the current buy order, set status as "New"
                            //Execution* transaction = new Execution;

                            transaction->client_Or_ID = order->Client_Order_ID;
                            //cout << transaction->client_Or_ID;
                            //transaction->order_ID = "ord" + to_string(i);
                            transaction->instrument = "Rose";  // Set the instrument correctly
                            transaction->price = order->Price; // Set the price correctly
                            transaction->quantity = order->Quantity; // Set the quantity correctly
                            transaction->status = "New";
                            report.push_back(transaction);
                            //Buy_Rose.erase(Buy_Rose.begin());
                            
                        }

                    }
                    else {
                        bool matched = false;
                        for (Order_Book* buy_order : Buy_Rose) {
                            //cout << order->Client_Order_ID << endl;
                            //bool matched = false;
                            

                            if (order->Price <= buy_order->price) {


                                
                                //cout << transaction->client_Or_ID << endl;
                                //transaction->order_ID = "ord" + to_string(i);
                                

                                if (order->Quantity == buy_order->qty) {
                                    Execution* sell_side = new Execution;
                                    sell_side->client_Or_ID = order->Client_Order_ID;
                                    sell_side->status = "Fill";
                                    sell_side->order_ID = order->Ord_id;;
                                    sell_side->quantity = buy_order->qty; // Set the quantity correctly
                                    sell_side->instrument = "Rose";  // Set the instrument correctly
                                    sell_side->price = order->Price; // Set the price correctly
                                    report.push_back(sell_side);
                                    
                                    Execution* buy_side = new Execution;
                                    buy_side->order_ID = "ord" + to_string(i);
                                    buy_side->status = "Fill";
                                    buy_side->quantity = buy_order->qty;
                                    buy_side->instrument = "Rose";  // Set the instrument correctly
                                    buy_side->price = order->Price;
                                    buy_side->client_Or_ID = buy_order->id;
                                    report.push_back(buy_side);
                                    matched = 1;
                                    Sell_Rose.erase(Sell_Rose.begin());
                                    Buy_Rose.erase(Buy_Rose.begin());

                                    break;
                                }
                                else if (order->Quantity < buy_order->qty) {
                                    transaction->status = "Fill";
                                    transaction->quantity = order->Quantity; // Set the quantity correctly
                                    buy_order->qty -= order->Quantity;
                                    transaction->client_Or_ID = order->Client_Order_ID;
                                    transaction->price = order->Price;
                                    cout << buy_order->qty << endl;
                                    Sell_Rose.erase(Sell_Rose.begin());
                                    report.push_back(transaction);

                                    Execution* buy_side = new Execution;
                                    buy_side->status = "Pfill";
                                    buy_side->quantity = order->Quantity;
                                    buy_side->instrument = "Rose";  // Set the instrument correctly
                                    buy_side->price = order->Price;
                                    buy_side->client_Or_ID = buy_order->id;
                                    //Buy_Rose.erase(Buy_Rose.begin());
                                    report.push_back(buy_side);
                                    matched = true;
                                    break;
                                }
                                else { // buy_order->qty < sell_order->qty
                                    transaction->status = "PFill";
                                    transaction->quantity = buy_order->qty; // Set the quantity correctly
                                    order->Quantity -= buy_order->qty;
                                    transaction->client_Or_ID = order->Client_Order_ID;
                                    transaction->price = order->Price;
                                    report.push_back(transaction);

                                    Execution* buy_side = new Execution;
                                    buy_side->status = "Fill";
                                    buy_side->quantity = buy_order->qty;
                                    buy_side->instrument = "Rose";  // Set the instrument correctly
                                    buy_side->price = order->Price;
                                    buy_side->client_Or_ID = buy_order->id;
                                    Buy_Rose.erase(Buy_Rose.begin());
                                    report.push_back(buy_side);
                                    matched = true;
                                   break;

                                }
                            }

                            
                           
                        }
                        //cout << matched;
                        if (matched==0) {
                            //cout<< matched;
                            // If no match occurred for the current buy order, set status as "New"
                            //Execution* transaction = new Execution;

                            transaction->client_Or_ID = order->Client_Order_ID;
                            //cout << transaction->client_Or_ID;
                            transaction->order_ID = order->Ord_id;;
                            transaction->instrument = "Rose";  // Set the instrument correctly
                            transaction->price = order->Price; // Set the price correctly
                            transaction->quantity = order->Quantity; // Set the quantity correctly
                            transaction->status = "New";
                            report.push_back(transaction);
                            //Buy_Rose.erase(Buy_Rose.begin());
                            

                        }
                    }

                }

            }
            i++;



        }
        

        
        for (const Execution* j : report) {
            cout << "Client Order ID: " << j->client_Or_ID << "| Order ID: " << j->order_ID << " | Instrument: " << j->instrument
                << " | Quantity: " << j->quantity
                << " | Price: " << j->price << " | Status: " << j->status << " | Reason: " << j->reason << endl;
        }

        // Clean up dynamically allocated memory
        for (Order_Book* buy_order : Buy_Rose) {
            delete buy_order;
        }
        for (Order_Book* sell_order : Sell_Rose) {
            delete sell_order;
        }
    }


};

int main() {
    Flower_Trading Trading_app;
    string File_Path = "ex2.csv";

    Trading_app.Read_CSV(File_Path);
    Trading_app.Process_Order();

    return 0;
}
