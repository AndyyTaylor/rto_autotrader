
#include <iostream>

#include "autotrader.h"

constexpr char EXEC_HOST[] = "127.0.0.1";
constexpr int EXEC_PORT = 12341;
constexpr char NAME[] = "andy";
constexpr char SECRET[] = "password";

int main() {
    std::cout << "test" << '\n';

    AutoTrader trader = AutoTrader(NAME, SECRET, EXEC_HOST, EXEC_PORT);

    return 0;
}