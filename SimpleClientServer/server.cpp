// ChatWithServer_Server_Part.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<WinSock2.h>
#include<WS2tcpip.h>
#include <iostream>
#include <string>

#include "Error.h"

#pragma comment(lib, "ws2_32.lib")

#define MESSAGE_LENGTH 256
#define MAX_SOCKETS 5
#define PORT 8585

int main()
{
    WSAData ws;
    // Loading DLL for Socket API
    int errWS = WSAStartup(MAKEWORD(2, 2), &ws);

    if(errWS!=0)
    {
        Error("The program could not initialize the Socket Lib ...");
    }

    int addrFamily = AF_INET;
    int sockType = SOCK_STREAM;
    int protocolo = IPPROTO_TCP;
    int masterSocket = socket(addrFamily, sockType, protocolo);

    if (masterSocket == INVALID_SOCKET)
    {
        Error("The program failed to create the Master Socket ... "+ WSAGetLastError());
    }

    // Bind the Server Code to a Port
    sockaddr_in srv_address;
    srv_address.sin_family = addrFamily;
    srv_address.sin_port = htons(PORT);
    srv_address.sin_addr.s_addr = INADDR_ANY;// if you want to bind your local IP address. My machine is the server
    memset(&srv_address.sin_zero, 0, sizeof(srv_address.sin_zero));
    int err_bind = bind(masterSocket, (sockaddr*)&srv_address, sizeof(srv_address));
    if (err_bind == SOCKET_ERROR)
    {
        Error("The program failed to bind ... " + WSAGetLastError());
    }

    int err_listen = listen(masterSocket, MAX_SOCKETS);// If no error occurs, listen returns zero. Otherwise, a value of SOCKET_ERROR is returned
    if (err_listen == SOCKET_ERROR)
    {
        Error("The program was not able to listen ... " + WSAGetLastError());
    }

    //Accept a new connection
    sockaddr_in clientInfo; // buffer that receives the address of the connecting entity
    int clientInfoLen = sizeof(clientInfo);
    
    Log("Waiting for connection ...");

    //Accept create a new socket
    int newSocket = accept(masterSocket, (sockaddr*)&clientInfo, &clientInfoLen);
    
    if (newSocket == INVALID_SOCKET)
    {
        Error("The program failed to accept a new connection ..."+ WSAGetLastError());
    }
    else
    {
        Log("Connected with a new Client ...");
        
        std::string IP;
        std::string IP_INFO = "IP ADDRESS: ";
        IP_INFO+= inet_ntop(AF_INET, &clientInfo.sin_addr, &IP[0], sizeof(IP));

        Log(IP_INFO.c_str());
    }

    bool running = true;
    while (running)
    {
        // -----------------------------
        // Receive Message from Client

        std::cout << std::endl<<"Please, wait for client response ..." << std::endl;
        std::string clientMsg;
        clientMsg.resize(MESSAGE_LENGTH);
        int bytesReceived = recv(newSocket, &clientMsg[0], MESSAGE_LENGTH, 0);
        if (bytesReceived < 0)
        {
            Error("The program failed to read the message from Client ...");
        }
        std::cout << std::endl<<"CLIENT Said:: " << clientMsg<< std::endl;
        
        // ------------------------
        // Sending Message to Client

        std::cout << "Write your message:> ";
        std::string serverMsg;
        serverMsg.resize(MESSAGE_LENGTH);
        std::getline(std::cin, serverMsg);
        int bytesSent= send(newSocket, &serverMsg[0], MESSAGE_LENGTH, 0);
        if (bytesSent < 0)
        {
            Error("The Server failed to send message to CLIENT...");
        }
        if (serverMsg == "exit" || serverMsg == "bye")
        {
            running = false;
        }
    }

    closesocket(newSocket);
    closesocket(masterSocket);

    std::cout << std::endl<<"***** Connection Closed *****\n";
    std::cout << std::endl << "... Press <ENTER> to exit the program ..." << std::endl;
    std::cin.get();

    return 0;
}

