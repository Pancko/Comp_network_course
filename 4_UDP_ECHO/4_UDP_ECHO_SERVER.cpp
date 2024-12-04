//#undef UNICODE
//
//#define WIN32_LEAN_AND_MEAN
//
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <thread>
//#include <iostream>
//
//// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
//// #pragma comment (lib, "Mswsock.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "80"
//#pragma warning(disable : 4996)
//
//int __cdecl main(void)
//{
//    WSADATA wsaData;
//    SOCKET Socket = INVALID_SOCKET;
//    struct sockaddr_in SocketAddress, ClientAddress;
//    char buffer[DEFAULT_BUFLEN];
//    int iResult;
//
//    // Initialize Winsock
//    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (iResult != 0) {
//        printf("WSAStartup failed with error: %d\n", iResult);
//        return 1;
//    }
//
//    if ((Socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//    {
//        printf("socket error%d\n", WSAGetLastError());
//        return 0;
//    }
//
//    ZeroMemory(&SocketAddress, sizeof(SocketAddress));
//    SocketAddress.sin_family = AF_INET;
//    SocketAddress.sin_port = htons(12345); // Функция htons преобразует u_short из узла в порядок байтов сети TCP/IP (большой байт).
//    SocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
//
//    iResult = bind(Socket, (SOCKADDR*)&SocketAddress, sizeof(SocketAddress));
//    if (iResult != 0) {
//        printf("bind failed with error %d\n", WSAGetLastError());
//        return 1;
//    }
//
//    while (true) // Слушаем и принимаем клиента
//    {
//        int size_ClientAddress = sizeof(ClientAddress);
//
//        int bytesReceived = recvfrom(Socket, buffer, DEFAULT_BUFLEN, 0, (SOCKADDR*)&ClientAddress, &size_ClientAddress);
//        if (bytesReceived > 0) {
//            buffer[bytesReceived] = '\0';
//            std::cout << "Received: " << buffer << std::endl;
//            sendto(Socket, buffer, bytesReceived, 0, (SOCKADDR*)&ClientAddress, size_ClientAddress);
//        }
//        else return 1;
//    }
//
//    WSACleanup();
//
//    return 0;
//}