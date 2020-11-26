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
    // TODO(andy@ataylor.xyz)
}

// void AutoTrader::positionUpdate(int future_position, int etf_position) {
//     etf_position_ = etf_position;
// }

// void AutoTrader::orderStatusUpdate(int id, int fill_volume, int remaining_volume, int fees) {
//     if (remaining_volume == 0) {
//         if (id == bid_id_) {
//             bid_id_ = 0;
//         } else if (id == ask_id_) {
//             ask_id_ = 0;
//         }

//         if (bid_volumes_.find(id) != bid_volumes_.end()) {
//             bid_volumes_.erase(id);
//         } else {
//             ask_volumes_.erase(id);
//         }
//     } else {
//         if (bid_volumes_.find(id) != bid_volumes_.end()) {
//             bid_volumes_[id] = remaining_volume;
//         } else {
//             ask_volumes_[id] = remaining_volume;
//         }
//     }
// }

// int AutoTrader::clampPrice(int price) {
//     return price / 100 * 100;
// }

// void AutoTrader::insertOrder(int id, Side side, int price, int volume, Lifespan lifespan) {
//     auto size = 17;

//     auto msg = messages::Message{};
//     msg.size = htons(size);
//     msg.type = 4;
//     msg.insert_order.id = htonl(id);
//     msg.insert_order.side = side;
//     msg.insert_order.price = htonl(price);
//     msg.insert_order.volume = htonl(volume);
//     msg.insert_order.lifespan = lifespan;

//     send(exec_sock_, &msg, size, 0);
// }

// void AutoTrader::cancelOrder(int id) {
//     auto size = 7;

//     auto msg = messages::Message{};
//     msg.size = htons(size);
//     msg.type = 2;
//     msg.cancel_order.id = htonl(id);

//     send(exec_sock_, &msg, size, 0);
// }

// unsigned char *buffer=reinterpret_cast<unsigned char *>(malloc(sizeof(msg)));
// int i;

// //copying....
// memcpy(buffer,(const unsigned char*)&msg,sizeof(msg));

// //printing..
// printf("Copied byte array is:\n");
// for(i=0;i<sizeof(msg);i++)
// printf("%d ",buffer[i]);
// printf("\n");

// //freeing memory..
// free(buffer);

// std::cout << ntohl(msg.id) << '\n';
