// Copyright 2020 Andy Taylor

#ifndef SRC_PRICING_PRICING_MODEL_H_
#define SRC_PRICING_PRICING_MODEL_H_

#include <iostream>
#include <vector>

class PricingModel {
 public:
    virtual int calculateTheoPrice() = 0;

    virtual void onOrderBookUpdate(
        Instrument instrument, int sequence_no,
        const std::vector<int>& ask_prices, const std::vector<int>& ask_volumes,
        const std::vector<int>& bid_prices, const std::vector<int>& bid_volumes)
        {}
};

#endif  // SRC_PRICING_PRICING_MODEL_H_
