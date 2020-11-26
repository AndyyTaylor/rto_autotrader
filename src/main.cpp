// Copyright 2020 Andy Taylor

#define BUFFER_SIZE 4096

#include <cstring>
#include <iostream>
#include <vector>

#include "autotrader/autotrader.h"
#include "market_link/market_link.h"
#include "constants/messages.h"

constexpr char EXEC_HOST[] = "127.0.0.1";
constexpr int EXEC_PORT = 12341;

constexpr char INFO_HOST[] = "239.255.1.1";
constexpr int INFO_PORT = 12346;

constexpr char NAME[] = "andy";
constexpr char SECRET[] = "password";

int main() {
    auto market_link = MarketLink(EXEC_HOST, EXEC_PORT, INFO_HOST, INFO_PORT);
    auto trader = AutoTrader(NAME, SECRET);

    try {
        market_link.initConnection();
        market_link.registerTrader(&trader);
        trader.registerMarketLink(&market_link);

        market_link.runLoop();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch(...) {
        std::cerr << "unknown error\n";
        return 1;
    }

    return 0;
}
