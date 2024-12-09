#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma warning(disable : 4996)

#define DEFAULT_BUFLEN 512
struct sockaddr_in SocketAddress; // Используется немного другая структура

void recieveMessageFromServer(SOCKET Socket)
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    int size_SocketAddress = sizeof(SocketAddress);

    while (TRUE)
    {
        // Receive until the peer closes the connection
        do {
            iResult = recvfrom(Socket, (char*)recvbuf, DEFAULT_BUFLEN, 0, (SOCKADDR*)&SocketAddress, &size_SocketAddress);
            if (iResult > 0)
            {
                printf("Bytes received: %d\n", iResult);
                recvbuf[iResult] = '\0'; // Добавить на конец принятого результата символ конца строки
                std::cout << recvbuf << std::endl;
            }
            else if (iResult == 0)
                printf("Connection closed\n");
        } while (iResult > 0);
    }
}

int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET Socket = INVALID_SOCKET;
    int iResult;
    const char* msg = "LITERALLY 1984";

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    if ((Socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) // SOCK_DGRAM потому что UDP использует пакеты-датаграммы вместо потока как в TCP
    {
        printf("socket error%d\n", WSAGetLastError());
        return 0;
    }

    ZeroMemory(&SocketAddress, sizeof(SocketAddress));
    SocketAddress.sin_family = AF_INET; // IPv4
    SocketAddress.sin_port = htons(12345); // Функция htons преобразует u_short из узла в порядок байтов сети TCP/IP (большой байт).
    SocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(Socket, (const char*)msg, strlen(msg), 0, (SOCKADDR*)&SocketAddress, sizeof(SocketAddress));

    std::thread thread_recieve(recieveMessageFromServer, Socket);
    thread_recieve.join();

    closesocket(Socket);
    WSACleanup();
    return 0;
}