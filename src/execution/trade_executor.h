// Copyright 2020 Andy Taylor

#ifndef SRC_EXECUTION_TRADE_EXECUTOR_H_
#define SRC_EXECUTION_TRADE_EXECUTOR_H_

#include <iostream>
#include <vector>

#include "constants/types.h"
#include "market_link/market_link.h"

class MarketLink;

class TradeExecutor {
 public:
    virtual void executeTrades(MarketLink* market_link, int bid_price, int ask_price) = 0;

    virtual void onOrderBookUpdate(
        Instrument instrument, int sequence_no,
        const std::vector<int>& ask_prices, const std::vector<int>& ask_volumes,
        const std::vector<int>& bid_prices, const std::vector<int>& bid_volumes)
        {}

    virtual void onPositionUpdate(int future_position, int etf_position) {}
};

#endif  // SRC_EXECUTION_TRADE_EXECUTOR_H_
