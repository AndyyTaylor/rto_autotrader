
#include "autotrader.h"

#include <cstring>
#include <iostream>
#include <unistd.h> 
#include <arpa/inet.h>
#include <vector>

void AutoTrader::login() {
    int size = 73;
    struct LoginMessage {
        unsigned short size;
        char type;
        char name[20];
        char secret[50];
    };

    LoginMessage msg;
    msg.size = htons(73);
    msg.type = 5;
    for (int i = 0; i < 20; i++) msg.name[i] = 0;
    for (int i = 0; i < 50; i++) msg.secret[i] = 0;
    strcpy(msg.name, name_.c_str());
    strcpy(msg.secret, secret_.c_str());

    send(exec_sock_, &msg, 73, 0);
}

void AutoTrader::on_order_book_update(int instrument, int sequence_no, std::vector<int>& ask_prices, std::vector<int>& ask_volumes, std::vector<int>& bid_prices, std::vector<int>& bid_volumes) {
    if (instrument == 1 || ask_prices.size() == 0 || bid_prices.size() == 0) {
        return;
    }

    int mid_price = clampPrice((bid_prices[0] + ask_prices[0]) / 2);
    int bid_price = mid_price - 100 - etf_position_ * 100;
    int ask_price = mid_price + 100 + etf_position_ * 100;

    insertOrder(1, ask_price, 1, 1);

    std::cout << bid_price << " - " << ask_price << '\n';
}

int AutoTrader::clampPrice(int price) {
    return price / 100 * 100;
}

void AutoTrader::insertOrder(int side, int price, int volume, int lifespan) {
    int size = 17;
    #pragma pack(push,1)
    struct InsertMessage {
        unsigned short size;
        char type;
        int id;
        char side;
        int price;
        int volume;
        char lifespan;
    };
    #pragma pack(pop)

    InsertMessage msg;
    msg.size = htons(size);
    msg.type = 4;
    msg.id = htonl(++order_id_);
    msg.side = side;
    msg.price = htonl(price);
    msg.volume = htonl(volume);
    msg.lifespan = lifespan;

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