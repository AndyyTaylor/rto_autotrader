
#ifndef AUTOTRADER_H_
#define AUTOTRADER_H_

#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string>

class AutoTrader {
    public:
        AutoTrader(const std::string& NAME, const std::string& SECRET, const std::string& EXEC_HOST, const int EXEC_PORT);

    private:
        void send_login_message();

        int exec_sock;
        int info_sock;
        const std::string NAME;
        const std::string SECRET;
};

#endif  // AUTOTRADER_H_