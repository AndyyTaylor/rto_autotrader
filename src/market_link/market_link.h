// Copyright 2020 Andy Taylor

#ifndef SRC_MARKET_LINK_MARKET_LINK_H_
#define SRC_MARKET_LINK_MARKET_LINK_H_

#include <string>
#include "autotrader/autotrader.h"
#include "constants/messages.h"

class AutoTrader;

// Handles connections between AutoTraders and the Exchange
// Sockets need to be created before the MarketLink can be used
// eg:
//      MarketLink marketLink = ...;
//      marketLink.initConnection();
//      ...
class MarketLink {
 public:
    MarketLink(const std::string& exec_host, int exec_port,
               const std::string& info_host, int info_port)
        : exec_host_{exec_host}, exec_port_{exec_port},
          info_host_{info_host}, info_port_{info_port} {}

    // Connects to the exchange's execution and information sockets
    void initConnection();
    // Registers the AutoTrader which will be sent market events
    void registerTrader(AutoTrader* auto_trader);
    // Runs the game loop
    void runLoop();

    void cancelOrder(int order_id);
    void insertOrder(int id, Side side, int price, int volume, Lifespan lifespan);

 private:
    int createExecutionSocket();
    int createInformationSocket();

    void readNextMessage(messages::Message& msg);
    void handleOrderBookUpdate(messages::Message& msg);
    void handleOrderStatusUpdate(messages::Message& msg);
    void handlePositionUpdate(messages::Message& msg);

    AutoTrader* auto_trader_;

    const std::string exec_host_;
    const std::string info_host_;
    int exec_port_;
    int info_port_;

    int exec_sock_;
    int info_sock_;
};


#endif  // SRC_MARKET_LINK_MARKET_LINK_H_
