
#include "autotrader.h"

#include <cstring>
#include <unistd.h> 

AutoTrader::AutoTrader(const std::string& NAME, const std::string& SECRET, const std::string& EXEC_HOST, const int EXEC_PORT) : NAME{NAME}, SECRET{SECRET} {
    if ((exec_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw "socket creation error";
    }
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(EXEC_PORT);

    if (inet_pton(AF_INET, EXEC_HOST.c_str(), &serv_addr.sin_addr) <= 0) { 
        throw "invalid address";
    } 

    if (connect(exec_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  { 
        throw "connection failed";
    }

    send_login_message();

    int valread;
    char buffer[1024] = {0};
    valread = read( exec_sock , buffer, 10); 
    printf("%s\n",buffer );
}

void AutoTrader::send_login_message() {
    int size = 73;
    struct LoginMessage {
        char size1;
        char size2;
        char type;
        char name[20];
        char secret[50];
    };

    LoginMessage msg;
    msg.size1 = 0;
    msg.size2 = 73;
    msg.type = 5;
    for (int i = 0; i < 20; i++) msg.name[i] = 0;
    for (int i = 0; i < 50; i++) msg.secret[i] = 0;
    strcpy(msg.name, NAME.c_str());
    strcpy(msg.secret, SECRET.c_str());

    send(exec_sock, &msg, msg.size2, 0);
}

// unsigned char *buffer=reinterpret_cast<unsigned char *>(malloc(sizeof(msg)));
// int i;

// //copying....
// memcpy(buffer,(const unsigned char*)&msg,sizeof(msg));

// //printing..
// printf("Copied byte array is:\n");
// for(i=0;i<sizeof(msg);i++)
// 	printf("%d ",buffer[i]);
// printf("\n");

// //freeing memory..
// free(buffer);