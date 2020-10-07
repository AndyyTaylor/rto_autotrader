
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
    std::cout << "(" << instrument << ") " << sequence_no << ": ";
    for (int i = 0; i < ask_prices.size(); i++) {
        std::cout << ask_prices[i] << ", ";
    }
    std::cout << " | ";
    for (int i = 0; i < bid_volumes.size(); i++) {
        std::cout << bid_volumes[i] << ", ";
    }
    std::cout << '\n';
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