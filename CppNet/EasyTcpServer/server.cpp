#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define CHECKEQUALRET(ret, compare)\
if(ret == compare)\
{\
    std::cerr << "error ocurred in " << __FILE__\
              << "`s line " << __LINE__\
              << ", error " << ret;\
}

#define CHECKNEQUALRET(ret, compare)\
if(ret != compare)\
{\
    std::cerr << "error ocurred in " << __FILE__\
              << "`s line " << __LINE__\
              << ", error " << ret;\
}

typedef struct __NETDATA
{
    int type;
    char info[128];
}StruNetData;

int main()
{
    // Initialize the version environment
    auto version = MAKEWORD(2, 2);
    WSADATA data = { 0 };
    auto ret = WSAStartup(version, &data);
    CHECKNEQUALRET(ret, 0);

    // Create a socket and listen for requests
    auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECKEQUALRET(sock, INVALID_SOCKET);
    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_port = htons(4567);
    ret = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    CHECKEQUALRET(ret, SOCKET_ERROR);
    ret = listen(sock, 0);
    CHECKEQUALRET(ret, SOCKET_ERROR);
    sockaddr_in clientaddr = { 0 };
    int clientaddrlen = sizeof(clientaddr);
    SOCKET client = INVALID_SOCKET;

    // Handling client requests
    //std::string str = "hello, i am server.";
    StruNetData netdata = { 1, "hello, i am server." };
    while (true)
    {
        client = accept(sock, reinterpret_cast<sockaddr*>(&clientaddr), &clientaddrlen);
        CHECKEQUALRET(client, INVALID_SOCKET);
        std::cout << "new client connected : " << inet_ntoa(clientaddr.sin_addr) << std::endl;
        //ret = send(client, str.c_str(), str.size(), 0);
        ret = send(client, reinterpret_cast<const char*>(&netdata), sizeof(netdata), 0);
        std::cout << "sendt " << ret << " chars to client." << std::endl;
        ret = closesocket(client);
        CHECKEQUALRET(ret, SOCKET_ERROR);
    }
   
    ret = closesocket(sock);
    CHECKEQUALRET(ret, SOCKET_ERROR);

    // Clean up the environment
    ret = WSACleanup();
    CHECKNEQUALRET(ret, 0);
    return 0;
}