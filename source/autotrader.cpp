
#include "autotrader.h"
#include "messages.h"
#include "types.h"

#include <cstring>
#include <iostream>
#include <unistd.h> 
#include <chrono>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
#include <numeric>

void AutoTrader::orderBookUpdate(Instrument instrument, int sequence_no, std::vector<int>& ask_prices, std::vector<int>& ask_volumes, std::vector<int>& bid_prices, std::vector<int>& bid_volumes) {
    if (ask_prices.size() == 0 || bid_prices.size() == 0) {
        return;
    }

    if (instrument == ETF) {
        etf_bid_ = bid_prices[0];
        etf_ask_ = ask_prices[0];
    } else {
        theo_price_ = 1.0 * (bid_prices[0] + ask_prices[0]) / 2;
    }

    if (theo_price_ == 0) {
        return;
    }

    int indiff_price = theo_price_ - etf_position_ * 2.1;
    auto bid_price = clampPrice(indiff_price - 100);
    auto ask_price = clampPrice(indiff_price + 100);

    if (bid_price >= ask_price) {
        if (etf_position_ > 0) {
            bid_price -= 100;
        } else {
            ask_price += 100;
        }
    }

    if (etf_bid_ != 0 && etf_ask_ != 0) {
        if (bid_price > etf_bid_ + 100) {
            bid_price = etf_bid_ + 100;
        }

        if (ask_price < etf_ask_ - 100) {
            ask_price = etf_ask_ - 100;
        }
    }

    if (bid_id_ == 0 || bid_price != bid_price_) {
        if (bid_id_ != 0) {
            cancelOrder(bid_id_);
        }

        if (ask_price_ <= bid_price) {
            cancelOrder(ask_id_);
            ask_id_ = 0;
        }
    
        auto outstanding_volume = 0;
        for (auto kv : bid_volumes_) {
            outstanding_volume += kv.second;
        }
        auto volume = std::min(50, 100 - etf_position_ - outstanding_volume);

        if (volume > 0) {
            bid_id_ = ++order_id_;
            bid_volumes_[bid_id_] = volume;
            bid_price_ = bid_price;
            insertOrder(bid_id_, BUY, bid_price_, volume, GOOD_FOR_DAY);
        }
    }

    if (ask_id_ == 0 || ask_price != ask_price_) {
        if (ask_id_ != 0) {
            cancelOrder(ask_id_);
        }

        auto outstanding_volume = 0;
        for (auto kv : ask_volumes_) {
            outstanding_volume += kv.second;
        }
        auto volume = std::min(50, 100 + etf_position_ - outstanding_volume);

        if (volume > 0) {
            ask_id_ = ++order_id_;
            ask_volumes_[ask_id_] = volume;
            ask_price_ = ask_price;
            insertOrder(ask_id_, SELL, ask_price_, volume, GOOD_FOR_DAY);
        }
    }

    std::cout << indiff_price << ": " <<  bid_price << ", " << ask_price << " # " << bid_price_ << " - " << ask_price_ << " (" << etf_position_ << ")\n";
}

void AutoTrader::positionUpdate(int future_position, int etf_position) {
    etf_position_ = etf_position;
}

void AutoTrader::orderStatusUpdate(int id, int fill_volume, int remaining_volume, int fees) {
    if (remaining_volume == 0) {
        if (id == bid_id_) {
            bid_id_ = 0;
        } else if (id == ask_id_) {
            ask_id_ = 0;
        }

        if (bid_volumes_.find(id) != bid_volumes_.end()) {
            bid_volumes_.erase(id);
        } else {
            ask_volumes_.erase(id);
        }
    } else {
        if (bid_volumes_.find(id) != bid_volumes_.end()) {
            bid_volumes_[id] = remaining_volume;
        } else {
            ask_volumes_[id] = remaining_volume;
        }
    }
}

int AutoTrader::clampPrice(int price) {
    return price / 100 * 100;
}

void AutoTrader::login() {
    auto msg = messages::Message{};

    msg.size = htons(73);
    msg.type = 5;
    for (auto i = 0; i < 20; i++) msg.login.name[i] = 0;
    for (auto i = 0; i < 50; i++) msg.login.secret[i] = 0;
    strcpy(msg.login.name, name_.c_str());
    strcpy(msg.login.secret, secret_.c_str());

    send(exec_sock_, &msg, 73, 0);
}

void AutoTrader::insertOrder(int id, Side side, int price, int volume, Lifespan lifespan) {
    auto size = 17;

    auto msg = messages::Message{};
    msg.size = htons(size);
    msg.type = 4;
    msg.insert_order.id = htonl(id);
    msg.insert_order.side = side;
    msg.insert_order.price = htonl(price);
    msg.insert_order.volume = htonl(volume);
    msg.insert_order.lifespan = lifespan;

    send(exec_sock_, &msg, size, 0);
}

void AutoTrader::cancelOrder(int id) {
    auto size = 7;

    auto msg = messages::Message{};
    msg.size = htons(size);
    msg.type = 2;
    msg.cancel_order.id = htonl(id);

    send(exec_sock_, &msg, size, 0);
}

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