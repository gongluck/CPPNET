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

typedef enum { UNKNOW, LOGIN, LOGINRET } ProtocolType;
typedef struct __PROTOCOLHEADER
{
    ProtocolType type;
    uint32_t bodylen;
}ProtocolHeader;
typedef struct __LOGININFO
{
    char user[16];
    char password[32];
}LoginInfo;
typedef struct __LOGINRESULT
{
    uint16_t ret;
    char msg[32];
}LoginResult;

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
    ProtocolHeader header;
    while (true)
    {
        client = accept(sock, reinterpret_cast<sockaddr*>(&clientaddr), &clientaddrlen);
        CHECKEQUALRET(client, INVALID_SOCKET);
        std::cout << "new client connected : " << inet_ntoa(clientaddr.sin_addr) << std::endl;
        ret = recv(client, reinterpret_cast<char*>(&header), sizeof(header), 0);
        CHECKNEQUALRET(ret, sizeof(header));
        if (ret != sizeof(header))
        {
            ret = closesocket(client);
            CHECKEQUALRET(ret, SOCKET_ERROR);
            continue;
        }
        std::cout << "header.type is " << header.type
            << ", header.bodylen is " << header.bodylen << std::endl;
        switch (header.type)
        {
        case LOGIN:
        {
            LoginInfo logininfo;
            ret = recv(client, reinterpret_cast<char*>(&logininfo), sizeof(logininfo), 0);
            CHECKNEQUALRET(ret, sizeof(logininfo));
            if (ret != sizeof(logininfo))
            {
                ret = closesocket(client);
                CHECKEQUALRET(ret, SOCKET_ERROR);
                continue;
            }
            std::cout << "logininfo.user is " << logininfo.user
                << ", logininfo.password is " << logininfo.password << std::endl;
            LoginResult loginresult = { 0, "login succeed." };
            header.type = LOGINRET;
            header.bodylen = sizeof(loginresult);
            ret = send(client, reinterpret_cast<char*>(&header), sizeof(header), 0);
            CHECKNEQUALRET(ret, sizeof(header));
            ret = send(client, reinterpret_cast<char*>(&loginresult), sizeof(loginresult), 0);
            CHECKNEQUALRET(ret, sizeof(loginresult));
        }
        break;
        default:
            break;
        }
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