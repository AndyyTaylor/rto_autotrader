// Copyright 2020 Andy Taylor

#ifndef SRC_CONSTANTS_MESSAGES_H_
#define SRC_CONSTANTS_MESSAGES_H_

#include <cstdint>

namespace messages {

constexpr int LOGIN_MESSAGE_SIZE = 73;
constexpr int LOGIN_MESSAGE_TYPE = 5;
constexpr int ORDER_BOOK_UPDATE_MESSAGE_TYPE = 6;

// Ensures no padding is added between struct fields, which matches
// representation used during transit
#pragma pack(push, 1)

struct Login {
    char name[20];
    char secret[50];
};

struct OrderBookUpdate {
    char instrument;
    unsigned int sequence_no;
    unsigned int prices[20];
};

struct PositionUpdate {
    int future_position;
    int etf_position;
};

struct OrderStatusUpdate {
    unsigned int id;
    unsigned int fill_volume;
    unsigned int remaining_volume;
    int fees;
};

struct InsertOrder {
    int id;
    char side;
    int price;
    int volume;
    char lifespan;
};

struct CancelOrder {
    int id;
};

struct Message {
    // size & type are included in all messages
    uint16_t size;
    unsigned char type;

    // the body of the message varies based on type
    union {
        Login login;
        InsertOrder insert_order;
        CancelOrder cancel_order;
        PositionUpdate position_update;
        OrderBookUpdate order_book_update;
        OrderStatusUpdate order_status_update;
    };
};

#pragma pack(pop)

}  // namespace messages

#endif  // SRC_CONSTANTS_MESSAGES_H_
