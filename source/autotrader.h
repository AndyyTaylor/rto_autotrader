
#ifndef AUTOTRADER_H_
#define AUTOTRADER_H_

#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string>
#include <vector>
#include <unordered_map>

#include "types.h"

class AutoTrader {
    public:
        AutoTrader(const std::string& name, const std::string& secret, const int exec_sock)
            : name_{name}, secret_{secret}, exec_sock_{exec_sock}, etf_position_{0}, order_id_{0}, bid_id_{0}, ask_id_{0}, bid_price_{0}, ask_price_{0}, etf_ask_{0}, etf_bid_{0}, theo_price_{0} {};
        
        void login();
        void orderBookUpdate(Instrument instrument, int sequence_no, std::vector<int>& ask_prices, std::vector<int>& ask_volumes, std::vector<int>& bid_prices, std::vector<int>& bid_volumes);
        void positionUpdate(int future_position, int etf_position);
        void orderStatusUpdate(int id, int fill_volume, int remaining_volume, int fees);

    private:
        int clampPrice(int price);
        void insertOrder(int id, Side side, int price, int volume, Lifespan lifespan);
        void cancelOrder(int id);

        const int exec_sock_;
        const std::string name_;
        const std::string secret_;
        int etf_position_;
        int order_id_;
        int bid_id_;
        int ask_id_;
        int bid_price_;
        int ask_price_;

        int etf_bid_;
        int etf_ask_;
        double theo_price_;

        std::unordered_map<int, int> bid_volumes_;
        std::unordered_map<int, int> ask_volumes_;
};

#endif  // AUTOTRADER_H_