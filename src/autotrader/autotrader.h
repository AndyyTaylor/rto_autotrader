// Copyright 2020 Andy Taylor

#ifndef SRC_AUTOTRADER_AUTOTRADER_H_
#define SRC_AUTOTRADER_AUTOTRADER_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "constants/types.h"
#include "market_link/market_link.h"

class MarketLink;

class AutoTrader {
 public:
    AutoTrader(const std::string& name, const std::string& secret)
        : name_{name}, secret_{secret} {};

    // Registers the market link to send execution commands to
    void registerMarketLink(MarketLink* market_link);

    const std::string name() { return name_; }
    const std::string secret() { return secret_; }

    // Handles an order book update event from the exchange
    void onOrderBookUpdate(Instrument instrument, int sequence_no,
                           const std::vector<int>& ask_prices, const std::vector<int>& ask_volumes,
                           const std::vector<int>& bid_prices, const std::vector<int>& bid_volumes);

    // void positionUpdate(int future_position, int etf_position);
    // void orderStatusUpdate(int id, int fill_volume, int remaining_volume, int fees);

 private:
    // int clampPrice(int price);
    // void insertOrder(int id, Side side, int price, int volume, Lifespan lifespan);
    // void cancelOrder(int id);

    const std::string name_;
    const std::string secret_;

    MarketLink* market_link_;
    // int etf_position_;
    // int order_id_;
    // int bid_id_;
    // int ask_id_;
    // int bid_price_;
    // int ask_price_;

    // int etf_bid_;
    // int etf_ask_;
    // double theo_price_;

    // std::unordered_map<int, int> bid_volumes_;
    // std::unordered_map<int, int> ask_volumes_;
};

#endif  // SRC_AUTOTRADER_AUTOTRADER_H_
