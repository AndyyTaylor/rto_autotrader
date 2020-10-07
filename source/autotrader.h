
#ifndef AUTOTRADER_H_
#define AUTOTRADER_H_

#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string>
#include <vector>

class AutoTrader {
    public:
        AutoTrader(const std::string& name, const std::string& secret, const int exec_sock)
            : name_{name}, secret_{secret}, exec_sock_{exec_sock} {};
        
        void login();
        void on_order_book_update(int instrument, int sequence_no, std::vector<int>& ask_prices, std::vector<int>& ask_volumes, std::vector<int>& bid_prices, std::vector<int>& bid_volumes);

    private:
        const int exec_sock_;
        const std::string name_;
        const std::string secret_;
};

#endif  // AUTOTRADER_H_