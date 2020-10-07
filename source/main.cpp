
#define BUFFER_SIZE 4096

#include <iostream>
#include <unistd.h> 
#include <vector>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>

#include "autotrader.h"
#include "messages.h"

constexpr char EXEC_HOST[] = "127.0.0.1";
constexpr int EXEC_PORT = 12341;

constexpr char INFO_HOST[] = "239.255.1.1";
constexpr int INFO_PORT = 12346;

constexpr char NAME[] = "andy";
constexpr char SECRET[] = "password";

int createExecutionSocket(const std::string& host, const int port) {
    std::cout << "connecting to execution channel ... ";

    auto sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw "socket creation error";
    }
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) { 
        throw "invalid address";
    } 

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  { 
        throw "connection failed";
    }

    if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0) {
        throw "failed to set sock to non-blocking";
    }

    std::cout << "done\n";
    return sock;
}

int createInformationSocket(const std::string& host, const int port) {
    std::cout << "connecting to information channel ... ";

    auto sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        throw "socket creation error";
    }

    auto opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
        throw "setsockopt(SO_REUSEADDR) failed";
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) { 
        throw "invalid address";
    }

    if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  { 
        throw "connection failed";
    }

    if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0) {
        throw "failed to set sock to non-blocking";
    }

    std::cout << "done\n";
    return sock;
}

int main() {
    try {
        auto exec_sock = createExecutionSocket(EXEC_HOST, EXEC_PORT);
        auto info_sock = createInformationSocket(INFO_HOST, INFO_PORT);

        auto trader = AutoTrader(NAME, SECRET, exec_sock);
        trader.login();

        auto num_read = 0;
        auto msg = messages::Message{};
        while (num_read < 10000) {
            auto ret = read(exec_sock, &msg, sizeof(msg));
            if (ret < 0) {
                if (errno != EWOULDBLOCK) {
                    throw errno;
                }

                ret = read(info_sock, &msg, sizeof(msg));
                if (ret < 0) {
                    if (errno != EWOULDBLOCK) {
                        throw errno;
                    }

                    continue;
                }
            }

            msg.size = ntohs(msg.size);

            if (msg.type == 6) {
                auto order_book_update = msg.order_book_update;
                order_book_update.sequence_no = ntohl(order_book_update.sequence_no);

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

                trader.orderBookUpdate(order_book_update.instrument, order_book_update.sequence_no, book[0], book[1], book[2], book[3]);
            } else if (msg.type == 7) {
                auto order_status_update = msg.order_status_update;
                order_status_update.id = ntohl(order_status_update.id);
                order_status_update.fill_volume = ntohl(order_status_update.fill_volume);
                order_status_update.remaining_volume = ntohl(order_status_update.remaining_volume);
                order_status_update.fees = ntohl(order_status_update.fees);

                trader.orderStatusUpdate(order_status_update.id, order_status_update.fill_volume, order_status_update.remaining_volume, order_status_update.fees);
            } else if (msg.type == 8) {
                auto position_update = msg.position_update;
                position_update.etf_position = ntohl(position_update.etf_position);
                position_update.future_position = ntohl(position_update.future_position);

                trader.positionUpdate(position_update.future_position, position_update.etf_position);
            } else {
                std::cout << "unknown header type: " << (int) msg.type << " size: " << msg.size << '\n';
            }

            num_read++;
        }

    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch(...) {
        std::cerr << "unknown error\n";
        return 1;
    }

    return 0;
}