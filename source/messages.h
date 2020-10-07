
#ifndef MESSAGES_H_
#define MESSAGES_H_

namespace messages {

struct Header {
    unsigned short size;
    unsigned char type;
};

struct OrderBookUpdate {
    unsigned short size;
    unsigned char type;
    char instrument;
    unsigned int sequence_no;
    unsigned int prices[20];
};

}  // namespace messages

#endif  // MESSAGES_H_