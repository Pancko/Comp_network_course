#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <algorithm>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"
std::vector<SOCKET> clients; // ������� �� 2 ������� - ������ �������� ��� ����

static int serverFunc(SOCKET ClientSocket) // �������� � �������� ����������
{
    int iResult;
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    while (TRUE)
    {
        do
        {
            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {
                printf("Bytes received: %d\n", iResult);

                //Echo the buffer back to the sender
                for (auto client : clients)
                {
                    if (client != ClientSocket)
                        iSendResult = send(client, recvbuf, iResult, 0);
                    if (iSendResult == SOCKET_ERROR)
                    {
                        printf("send failed with error: %d\n", WSAGetLastError());
                        closesocket(ClientSocket);
                        WSACleanup();
                        return 1;
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
            }
            else {
                if (WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    Sleep(500);
                    continue;
                }
            }

        } while (iResult > 0);
    }

    closesocket(ClientSocket);
}


int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;// ����� ��� ������������� �������� ��������
    SOCKET ClientSocket = INVALID_SOCKET; // ����� �������

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; //Ipv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    //u_long mode = 1; // 1 to enable non-blocking socket
    //// The ioctlsocket function controls the I/O mode of a socket.
    //// FIONBIO - ������� ������������ ������ �������� ��� �������
    //ioctlsocket(ListenSocket, FIONBIO, &mode); // ������������� ���������� ������ non blocking i/o
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN); // �������� �������������
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    while (true) // ������� � ��������� �������
    {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL); // ��������� �������� ������� ������������ � ������ �������������
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        clients.push_back(ClientSocket);
        //ioctlsocket(ClientSocket, FIONBIO, &mode); // Set client socket to non-blocking

        std::thread thread(serverFunc, ClientSocket);
        thread.detach();
    }

    WSACleanup();
    return 0;
}
