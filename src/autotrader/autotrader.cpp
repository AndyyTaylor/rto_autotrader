// Copyright 2020 Andy Taylor

#include "autotrader/autotrader.h"

#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>  // NOLINT [build/c++11]
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "constants/messages.h"
#include "constants/types.h"
#include "market_link/market_link.h"

void AutoTrader::registerMarketLink(MarketLink* market_link) {
    assert(market_link != nullptr);

    market_link_ = market_link;
}

void AutoTrader::onOrderBookUpdate(
        Instrument instrument, int sequence_no,
        const std::vector<int>& ask_prices, const std::vector<int>& ask_volumes,
        const std::vector<int>& bid_prices, const std::vector<int>& bid_volumes) {
    pricing_model_->onOrderBookUpdate(instrument, sequence_no, ask_prices,
                                     ask_volumes, bid_prices, bid_volumes);

    int theo_price = pricing_model_->calculateTheoPrice();
    std::cout << "theo price: " << theo_price << "\n";

    executor_->executeTrades(market_link_, theo_price - 100, theo_price + 100);
}

void AutoTrader::onOrderStatusUpdate(int id, int fill_volume, int remaining_volume, int fees) {
    // TODO(andy@ataylor.xyz)
}

void AutoTrader::onPositionUpdate(int future_position, int etf_position) {
    executor_->onPositionUpdate(future_position, etf_position);
    // TODO(andy@ataylor.xyz)
}
