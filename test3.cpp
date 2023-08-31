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

    // ... (other functions)
    bool is_Valid_Order(const Order* order, Execution*& transaction) {
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


    void Process_Order() {
        vector<Order_Book*> Buy_Rose;
        vector<Order_Book*> Sell_Rose;

        int  i = 1;

        auto ascending = [](Order_Book* a, Order_Book* b) {
            return a->price < b->price;
        };

        auto descending = [](Order_Book* a, Order_Book* b) {
            return a->price > b->price;
        };

        for (Order* order : orders) {
            Execution* transaction = new Execution;
            //cout << order->Client_Order_ID << endl;
            //bool x= is_Valid_Order(order, transaction);

            //transaction->order_ID = "ord" + to_string(i);
            //transaction->client_Or_ID = order->Client_Order_ID;
            //report.push_back(transaction);
            if (!std::count(instrument_list.begin(), instrument_list.end(), order->Instrument)) {
                transaction->status = "Rejected";
                transaction->reason = "Instrument is not supported";
                transaction->client_Or_ID = order->Client_Order_ID;
                transaction->order_ID = "ord" + to_string(i);
                transaction->instrument = "Rose";
                transaction->price = order->Price;
                transaction->quantity = order->Quantity;
                //cout << order->Client_Order_ID << endl;
                report.push_back(transaction);
                bool x = false;
            }
            else if (order->Price < 0) {
                transaction->status = "Rejected";
                transaction->reason = "Invalid price";
                transaction->client_Or_ID = order->Client_Order_ID;
                transaction->order_ID = "ord" + to_string(i);
                transaction->instrument = "Rose";
                transaction->price = order->Price;
                transaction->quantity = order->Quantity;
                //cout << order->Client_Order_ID << endl;
                report.push_back(transaction);
                bool x = false;
            }
            else if (order->Quantity < 10 || order->Quantity > 1000) {
                transaction->status = "Rejected";
                transaction->reason = "Quantity is out of the range.";
                transaction->client_Or_ID = order->Client_Order_ID;
                transaction->order_ID = "ord" + to_string(i);
                transaction->instrument = "Rose";
                transaction->price = order->Price;
                transaction->quantity = order->Quantity;
                //cout << order->Client_Order_ID << endl;
                report.push_back(transaction);
                bool x = false;
            }
            else if (order->Quantity % 10 != 0) {
                transaction->status = "Rejected";
                transaction->reason = "Quantity must be a multiple of 10.";
                transaction->client_Or_ID = order->Client_Order_ID;
                transaction->order_ID = "ord" + to_string(i);
                //cout << order->Client_Order_ID << endl;
                transaction->instrument = "Rose";
                transaction->price = order->Price;
                transaction->quantity = order->Quantity;
                report.push_back(transaction);
                bool x = false;
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
                                    transaction->status = "PFill";
                                    transaction->quantity = order->Quantity; // Set the quantity correctly
                                    sell_order->qty -= order->Quantity;
                                    //Buy_Rose.erase(Buy_Rose.begin());
                                    report.push_back(transaction);
                                    matched = true;
                                    break;
                                }
                                else { // buy_order->qty > sell_order->qty
                                    transaction->status = "PFill";
                                    transaction->quantity = sell_order->qty; // Set the quantity correctly
                                    order->Quantity -= sell_order->qty;
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
                            transaction->order_ID = "ord" + to_string(i);
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
                                    Execution* transaction = new Execution;
                                    transaction->client_Or_ID = order->Client_Order_ID;
                                    transaction->status = "Fill";
                                    transaction->order_ID = "ord" + to_string(i);
                                    transaction->quantity = buy_order->qty; // Set the quantity correctly
                                    transaction->instrument = "Rose";  // Set the instrument correctly
                                    transaction->price = order->Price; // Set the price correctly
                                    report.push_back(transaction);
                                    
                                    Execution* transaction1 = new Execution;
                                    transaction1->order_ID = "ord" + to_string(i);
                                    transaction1->status = "Fill";
                                    transaction1->quantity = buy_order->qty;
                                    transaction1->instrument = "Rose";  // Set the instrument correctly
                                    transaction1->price = order->Price;
                                    transaction1->client_Or_ID = buy_order->id;
                                    report.push_back(transaction1);
                                    matched = 1;
                                    Sell_Rose.erase(Sell_Rose.begin());
                                    Buy_Rose.erase(Buy_Rose.begin());

                                    break;
                                }
                                //else if (order->Quantity < buy_order->qty) {
                                //    transaction->status = "PFill";
                                //    transaction->quantity = order->Quantity; // Set the quantity correctly
                                //    buy_order->qty -= order->Quantity;
                                //    //Buy_Rose.erase(Buy_Rose.begin());
                                //    report.push_back(transaction);
                                //    matched = true;
                                //    break;
                                //}
                                //else { // buy_order->qty > sell_order->qty
                                //    transaction->status = "PFill";
                                //    transaction->quantity = buy_order->qty; // Set the quantity correctly
                                //    order->Quantity -= buy_order->qty;
                                //    Sell_Rose.erase(Sell_Rose.begin());
                                //    report.push_back(transaction);
                                //    matched = true;
                                //    //break;

                                //}
                            }

                            
                           
                        }
                        //cout << matched;
                        if (matched==0) {
                            //cout<< matched;
                            // If no match occurred for the current buy order, set status as "New"
                            //Execution* transaction = new Execution;

                            transaction->client_Or_ID = order->Client_Order_ID;
                            //cout << transaction->client_Or_ID;
                            transaction->order_ID = "ord" + to_string(i);
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
    string File_Path = "C:\\Users\\user\\Desktop\\Gippa\\ex71.csv";

    Trading_app.Read_CSV(File_Path);
    Trading_app.Process_Order();

    return 0;
}
