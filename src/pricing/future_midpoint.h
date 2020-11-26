// Copyright 2020 Andy Taylor

#ifndef SRC_PRICING_FUTURE_MIDPOINT_H_
#define SRC_PRICING_FUTURE_MIDPOINT_H_

#include <vector>
#include "constants/types.h"
#include "pricing/pricing_model.h"

class FutureMidpoint : public PricingModel {
 public:
    int calculateTheoPrice() override;

    void onOrderBookUpdate(
        Instrument instrument, int sequence_no,
        const std::vector<int>& ask_prices, const std::vector<int>& ask_volumes,
        const std::vector<int>& bid_prices, const std::vector<int>& bid_volumes) override;

 private:
    int theo_price_;
};

#endif  // SRC_PRICING_FUTURE_MIDPOINT_H_
