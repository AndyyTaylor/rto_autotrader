// Copyright 2020 Andy Taylor

#include "pricing/future_midpoint.h"
#include <iostream>

int FutureMidpoint::calculateTheoPrice() {
    return theo_price_;
}

void FutureMidpoint::onOrderBookUpdate(
        Instrument instrument, int sequence_no,
        const std::vector<int>& ask_prices, const std::vector<int>& ask_volumes,
        const std::vector<int>& bid_prices, const std::vector<int>& bid_volumes) {
    if (instrument != Instrument::FUTURE || ask_prices.size() == 0 || bid_prices.size() == 0) {
        return;
    }

    theo_price_ = (ask_prices[0] + bid_prices[0]) / 2 / 100 * 100;
}
