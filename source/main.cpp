
#define BUFFER_SIZE 4096

#include <iostream>
#include <unistd.h> 
#include <vector>
#include <arpa/inet.h>
#include <cstring>

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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
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

    std::cout << "done\n";
    return sock;
}

int createInformationSocket(const std::string& host, const int port) {
    std::cout << "connecting to information channel ... ";

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        throw "socket creation error";
    }

    int opt = 1;
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
        messages::Header header;
        char buffer[BUFFER_SIZE];
        while (num_read < 30) {
            read(info_sock, &buffer, BUFFER_SIZE);

            memcpy(&header, buffer, sizeof(header));
            header.size = ntohs(header.size);

            if (header.type == 6) {
                messages::OrderBookUpdate msg;
                memcpy(&msg, buffer, sizeof(msg));
                msg.sequence_no = ntohl(msg.sequence_no);

                std::vector<std::vector<int>> book;
                for (int i = 0; i < 20; i++) {
                    if (i % 5 == 0) {
                        book.push_back(std::vector<int>{});
                    }

                    msg.prices[i] = ntohl(msg.prices[i]);

                    // being 0 is the same as it not existing
                    if (msg.prices[i] != 0) {
                        book.back().push_back(msg.prices[i]);
                    }
                }

                trader.on_order_book_update(msg.instrument, msg.sequence_no, book[0], book[1], book[2], book[3]);
            } else {
                std::cout << "unknown header type: " << (int) header.type << " size: " << header.size << '\n';
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