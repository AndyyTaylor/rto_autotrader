
#ifndef MESSAGES_H_
#define MESSAGES_H_

namespace messages {

// Ensures no padding is added between struct fields, which matches
// representation used during transit
#pragma pack(push,1)

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
    unsigned short size;
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

#endif  // MESSAGES_H_