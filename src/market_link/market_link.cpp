// Copyright 2020 Andy Taylor

#include "market_link/market_link.h"

#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>

#include "constants/messages.h"

void MarketLink::initConnection() {
    exec_sock_ = createExecutionSocket();
    info_sock_ = createInformationSocket();
}

// Stores a pointer to the AutoTrader and sends the
// login message to the exchange
void MarketLink::registerTrader(AutoTrader* auto_trader) {
    assert(auto_trader != nullptr);

    auto_trader_ = auto_trader;

    auto msg = messages::Message{};

    msg.size = htons(messages::LOGIN_MESSAGE_SIZE);
    msg.type = messages::LOGIN_MESSAGE_TYPE;

    std::fill(msg.login.name, msg.login.name + 20, 0);
    std::fill(msg.login.secret, msg.login.secret + 50, 0);

    snprintf(msg.login.name, auto_trader_->name().size() + 1, auto_trader_->name().c_str());
    snprintf(msg.login.secret, auto_trader_->secret().size() + 1, auto_trader_->secret().c_str());

    send(exec_sock_, &msg, messages::LOGIN_MESSAGE_SIZE, 0);
}

void MarketLink::runLoop() {
    auto msg = messages::Message{};
    while (1) {
        if (!readNextMessage(msg)) {
            continue;
        }

        std::cout << msg.type << "\n";

        switch (msg.type) {
            case messages::ORDER_BOOK_UPDATE_MESSAGE_TYPE: {
                handleOrderBookUpdate(msg);
            }
        }
        // if (msg.type == 6) {
        //     auto order_book_update = msg.order_book_update;
        //     order_book_update.sequence_no = ntohl(order_book_update.sequence_no);

        //     std::vector<std::vector<int>> book;
        //     for (int i = 0; i < 20; i++) {
        //         if (i % 5 == 0) {
        //             book.push_back(std::vector<int>{});
        //         }

        //         order_book_update.prices[i] = ntohl(order_book_update.prices[i]);

        //         // being 0 is the same as it not existing
        //         if (order_book_update.prices[i] != 0) {
        //             book.back().push_back(order_book_update.prices[i]);
        //         }
        //     }

        //     trader.orderBookUpdate(order_book_update.instrument == 0 ? FUTURE : ETF, order_book_update.sequence_no, book[0], book[1], book[2], book[3]);
        // } else if (msg.type == 7) {
        //     auto order_status_update = msg.order_status_update;
        //     order_status_update.id = ntohl(order_status_update.id);
        //     order_status_update.fill_volume = ntohl(order_status_update.fill_volume);
        //     order_status_update.remaining_volume = ntohl(order_status_update.remaining_volume);
        //     order_status_update.fees = ntohl(order_status_update.fees);

        //     trader.orderStatusUpdate(order_status_update.id, order_status_update.fill_volume, order_status_update.remaining_volume, order_status_update.fees);
        // } else if (msg.type == 8) {
        //     auto position_update = msg.position_update;
        //     position_update.etf_position = ntohl(position_update.etf_position);
        //     position_update.future_position = ntohl(position_update.future_position);

        //     trader.positionUpdate(position_update.future_position, position_update.etf_position);
        // } else {
        //     std::cerr << "unknown header type: " << (int) msg.type << " size: " << msg.size << '\n';
        // }
    }
}

// Attempts to read the next message from either the execution socket
// or the info socket, both are non-blocking.
// Returns true if a message has been read.
bool MarketLink::readNextMessage(messages::Message& msg) {
    auto ret = read(exec_sock_, &msg, sizeof(msg));
    if (ret < 0) {
        if (errno != EWOULDBLOCK) {
            throw errno;
        }

        ret = read(info_sock_, &msg, sizeof(msg));
        if (ret < 0) {
            if (errno != EWOULDBLOCK) {
                throw errno;
            }

            return false;
        }
    }

    msg.size = ntohs(msg.size);

    return true;
}

void MarketLink::handleOrderBookUpdate(messages::Message& msg) {
    auto order_book_update = msg.order_book_update;
    auto sequence_no = ntohl(order_book_update.sequence_no);

    std::vector<std::vector<int>> book;
    for (int i = 0; i < 20; i++) {
        if (i % 5 == 0) {
            book.push_back(std::vector<int>{});
        }

        order_book_update.prices[i] = ntohl(order_book_update.prices[i]);

        // being 0 is the same as it not existing
        if (order_book_update.prices[i] != 0) {
            book.back().push_back(order_book_update.prices[i]);
        }
    }

    Instrument instrument = order_book_update.instrument == 0 ? FUTURE : ETF;
    auto_trader_->onOrderBookUpdate(instrument, sequence_no, book[0], book[1], book[2], book[3]);
}

int MarketLink::createExecutionSocket() {
    std::cout << "connecting to execution channel ... ";

    auto sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error{"failed to create socket"};
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(exec_port_);

    if (inet_pton(AF_INET, exec_host_.c_str(), &serv_addr.sin_addr) <= 0) {
        throw std::runtime_error{"invalid address for socket"};
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error{"socket connection failed"};
    }

    if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0) {
        throw std::runtime_error{"failed to set sock to non-blocking"};
    }

    std::cout << "done\n";
    return sock;
}

int MarketLink::createInformationSocket() {
    std::cout << "connecting to information channel ... ";

    auto sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        throw std::runtime_error{"socket creation error"};
    }

    auto opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
        throw std::runtime_error{"setsockopt(SO_REUSEADDR) failed"};
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(info_port_);

    if (inet_pton(AF_INET, info_host_.c_str(), &serv_addr.sin_addr) <= 0) {
        throw std::runtime_error{"invalid address"};
    }

    if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  {
        throw std::runtime_error{"connection failed"};
    }

    if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0) {
        throw std::runtime_error{"failed to set sock to non-blocking"};
    }

    std::cout << "done\n";
    return sock;
}
