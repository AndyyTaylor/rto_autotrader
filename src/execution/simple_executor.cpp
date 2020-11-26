// Copyright 2020 Andy Taylor

#include "execution/simple_executor.h"
#include <iostream>

void SimpleExecutor::executeTrades(MarketLink* market_link, int bid_price, int ask_price) {
    if (bid_id_ != 0 && bid_price != bid_price_) {
        market_link->cancelOrder(bid_id_);
        bid_id_ = 0;
    }

    if (ask_id_ != 0 && ask_price != ask_price_) {
        market_link->cancelOrder(ask_id_);
        ask_id_ = 0;
    }

    if (bid_id_ == 0 && position_ < 80) {
        bid_id_ = ++order_id_;
        bid_price_ = bid_price;
        market_link->insertOrder(bid_id_, Side::BUY, bid_price_, 10, Lifespan::GOOD_FOR_DAY);
    }

    if (ask_id_ == 0 && position_ > -80) {
        ask_id_ = ++order_id_;
        ask_price_ = ask_price;
        market_link->insertOrder(ask_id_, Side::SELL, ask_price_, 10, Lifespan::GOOD_FOR_DAY);
    }
}

void SimpleExecutor::onPositionUpdate(int future_position, int etf_position) {
    position_ = etf_position;
}
