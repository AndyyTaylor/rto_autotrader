
#include "autotrader.h"
#include "messages.h"

#include <cstring>
#include <iostream>
#include <unistd.h> 
#include <arpa/inet.h>
#include <vector>

void AutoTrader::orderBookUpdate(int instrument, int sequence_no, std::vector<int>& ask_prices, std::vector<int>& ask_volumes, std::vector<int>& bid_prices, std::vector<int>& bid_volumes) {
    if (instrument == 1 || ask_prices.size() == 0 || bid_prices.size() == 0) {
        return;
    }

    auto mid_price = clampPrice((bid_prices[0] + ask_prices[0]) / 2);
    auto bid_price = mid_price - 100 - etf_position_ * 100;
    auto ask_price = mid_price + 100 - etf_position_ * 100;

    if (bid_id_ != 0 && bid_price != bid_price_) {
        cancelOrder(bid_id_);
        bid_id_ = 0;
    }

    if (ask_id_ != 0 && ask_price != ask_price_) {
        cancelOrder(ask_id_);
        ask_id_ = 0;
    }

    if (bid_id_ == 0 && etf_position_ < 100) {
        bid_id_ = ++order_id_;
        bid_price_ = bid_price;
        insertOrder(bid_id_, 1, bid_price, 1, 1);
    }

    if (ask_id_ == 0 && etf_position_ > -100) {
        ask_id_ = ++order_id_;
        ask_price_ = ask_price;
        insertOrder(ask_id_, 0, ask_price, 1, 1);
    }

    std::cout << bid_price << " - " << ask_price << " (" << etf_position_ << ")\n";
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

void AutoTrader::insertOrder(int id, int side, int price, int volume, int lifespan) {
    auto size = 17;

    auto msg = messages::Message{};
    msg.size = htons(size);
    msg.type = 4;
    msg.insert_order.id = htonl(id);
    msg.insert_order.side = side;
    msg.insert_order.price = htonl(price);
    msg.insert_order.volume = htonl(volume);
    msg.insert_order.lifespan = lifespan;

    std::cout << "placing " << id << " @ " << price << '\n';

    send(exec_sock_, &msg, size, 0);
}

void AutoTrader::cancelOrder(int id) {
    auto size = 7;

    auto msg = messages::Message{};
    msg.size = htons(size);
    msg.type = 2;
    msg.cancel_order.id = htonl(id);

    std::cout << "cancelling " << id << '\n';

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