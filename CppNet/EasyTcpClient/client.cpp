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
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(4567);
    ret = connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    CHECKEQUALRET(ret, SOCKET_ERROR);
    char buf[256] = { 0 };
    ret = recv(sock, buf, sizeof(buf), 0);
    if (ret > 0)
    {
        std::cout << buf << std::endl;
    }

    ret = closesocket(sock);
    CHECKEQUALRET(ret, SOCKET_ERROR);

    // Clean up the environment
    ret = WSACleanup();
    CHECKNEQUALRET(ret, 0);
    return 0;
}