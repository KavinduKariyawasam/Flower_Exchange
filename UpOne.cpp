void Process_Order() {
    // Create maps to store order book for buying and selling
    map<string, vector<Order_Book*>> Buy_Orders; // Map to store buy orders for each instrument
    map<string, vector<Order_Book*>> Sell_Orders; // Map to store sell orders for each instrument

    int i = 1; // For unique order number

    // Comparator functions for sorting order books
    auto ascending = [](Order_Book* a, Order_Book* b) {
        return a->price < b->price; // Sorts in ascending order of price
    };

    auto descending = [](Order_Book* a, Order_Book* b) {
        return a->price < b->price; // Sorts in descending order of price
    };

    // Loop through each order in the input
    for (const Order* order : orders) {
        // Create a transaction record for this order
        Execution* transaction = new Execution;

        // Check if the order is valid
        bool valid_order = is_Valid_Order(order, transaction);

        if (!valid_order) {
            // If the order is invalid, add the transaction to the report and skip further processing
            report.push_back(transaction);
            continue;
        }

        // Create an Order_Book entry for this order
        Order_Book* new_order = new Order_Book;
        new_order->id = order->Client_Order_ID;
        new_order->qty = order->Quantity;
        new_order->price = order->Price;

        // Add the order to the respective Buy_Orders or Sell_Orders map
        if (order->side == static_cast<int>(Side::Buy)) {
            Buy_Orders[order->Instrument].push_back(new_order);
        } else if (order->side == static_cast<int>(Side::Sell)) {
            Sell_Orders[order->Instrument].push_back(new_order);
        }

        // Update transaction details for this order
        transaction->client_Or_ID = order->Client_Order_ID;
        transaction->order_ID = "ord" + to_string(i);
        transaction->instrument = order->Instrument;
        transaction->price = order->Price;
        transaction->quantity = order->Quantity;
        transaction->status = "New";

        // Add the transaction to the report
        report.push_back(transaction);

        i++; // Increment the unique order number
    }

    // Process matching orders for each instrument
    for (auto& instrument_orders : Buy_Orders) {
        string instrument = instrument_orders.first;
        vector<Order_Book*>& buy_orders = instrument_orders.second;
        vector<Order_Book*>& sell_orders = Sell_Orders[instrument];

        // Sort buy and sell orders based on price
        sort(buy_orders.begin(), buy_orders.end(), descending);
        sort(sell_orders.begin(), sell_orders.end(), ascending);

        // Match buy and sell orders
        for (Order_Book* buy_order : buy_orders) {
            for (Order_Book* sell_order : sell_orders) {
                if (sell_order->price >= buy_order->price) {
                    int match_qty = min(sell_order->qty, buy_order->qty);

                    // Get corresponding transactions for buy and sell orders
                    Execution* buy_transaction = GetTransactionByID(buy_order->id);
                    Execution* sell_transaction = GetTransactionByID(sell_order->id);

                    // Update transaction statuses and quantities based on the match
                    if (match_qty == sell_order->qty) {
                        buy_transaction->status = "Fill";
                        sell_transaction->status = "Fill";
                        sell_orders.erase(sell_orders.begin());
                    } else {
                        buy_transaction->status = "Partially Fill";
                        sell_transaction->status = "Partially Fill";
                        sell_order->qty -= match_qty;
                    }

                    buy_transaction->quantity = match_qty;
                    sell_transaction->quantity = match_qty;

                    buy_order->qty -= match_qty;

                    if (buy_order->qty == 0) {
                        break; // Move to the next buy order if the current one is fully filled
                    }
                } else {
                    break; // Exit the loop if no more suitable sell orders for this buy order
                }
            }

            if (buy_order->qty > 0) {
                // If there's remaining quantity in the buy order, add it back to the order book
                Buy_Orders[instrument].push_back(buy_order);
            }
        }
    }

    // Print the execution report
    for (const Execution* j : report) {
        cout << "Client Order ID: " << j->client_Or_ID << " | Order ID: " << j->order_ID << " | Instrument: " << j->instrument
            << " | Quantity: " << j->quantity
            << " | Price: " << j->price << " | Status: " << j->status << " | Reason: " << j->reason << endl;
    }
}
