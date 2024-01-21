#include <iostream>
#include <string>
#include<WinSock2.h>
#include<WS2tcpip.h>

#include "Error.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 8585
#define MESSAGE_LENGTH 256

int main()
{
    WSADATA ws;

    int errWS = WSAStartup(MAKEWORD(2, 2), &ws);

    if (errWS != 0)
    {
        Error("The program could not initialize the Socket Lib ...");
    }
    
    int clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSock == INVALID_SOCKET)
    {
        Error("The program failed to create the Master Socket..." + WSAGetLastError());
    }
    
    sockaddr_in srv;

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    PCWSTR SERVER_IP = TEXT("192.168.15.174");
    InetPton(srv.sin_family, SERVER_IP, &srv.sin_addr.s_addr);

    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

    int err_connect = connect(clientSock, (sockaddr*)&srv, sizeof(srv));
    if (err_connect == SOCKET_ERROR)
    {
        Error("Connection failed ..." + WSAGetLastError());
    }
    else
    {
        Log("Successfull connection ...");
    }

    bool running = true;
    while (running)
    {
        std::string clientMsg;
        clientMsg.resize(MESSAGE_LENGTH);
        std::cout << "Write your message:> ";
        std::getline(std::cin, clientMsg);
        int bytesSent = send(clientSock, &clientMsg[0], MESSAGE_LENGTH, 0);
        if (bytesSent < 0)
        {
            Error("Client failed to send message to SERVER...");
        }

        std::cout << std::endl<<"Please, wait for server response ..." << std::endl;

        std::string serverMsg;
        serverMsg.resize(MESSAGE_LENGTH);
        int bytesReceived = recv(clientSock, &serverMsg[0], MESSAGE_LENGTH, 0);
        if (bytesReceived < 0)
        {
            Error("The program failed to read messagem from Server ...");
        }
        std::cout << std::endl<<"SERVER Said:: " << serverMsg << std::endl;
        
        int res1 = strncmp("exit", serverMsg.c_str(), 4);
        int res2 = strncmp("bye", serverMsg.c_str(), 3);
    
        if((res1==0)||(res2==0))
        {
            running = false;
        }

    }

    closesocket(clientSock);
    
    std::cout << std::endl << "***** Connection Closed by the SERVER *****" << std::endl;
    std::cout << std::endl << "... Press <ENTER> to exit the program ..." << std::endl;

    std::cin.get();

    return 0;
}

