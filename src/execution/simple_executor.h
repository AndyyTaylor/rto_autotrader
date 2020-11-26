// Copyright 2020 Andy Taylor

#ifndef SRC_EXECUTION_SIMPLE_EXECUTOR_H_
#define SRC_EXECUTION_SIMPLE_EXECUTOR_H_

#include <vector>
#include "constants/types.h"
#include "execution/trade_executor.h"
#include "market_link/market_link.h"

class SimpleExecutor : public TradeExecutor {
 public:
    void executeTrades(MarketLink* market_link, int bid_price, int ask_price) override;

    void onPositionUpdate(int future_position, int etf_position) override;

 private:
    int bid_id_, ask_id_;
    int bid_price_, ask_price_;

    int order_id_ = 1;
    int position_;
};

#endif  // SRC_EXECUTION_SIMPLE_EXECUTOR_H_
