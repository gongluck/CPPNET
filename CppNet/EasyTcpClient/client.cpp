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
    uint32_t totallen;
}ProtocolHeader;
typedef struct __LOGININFO
{
    ProtocolHeader header = { LOGIN, sizeof(__LOGININFO) };
    char user[16];
    char password[32];
}LoginInfo;
typedef struct __LOGINRESULT
{
    ProtocolHeader header = { LOGINRET, sizeof(__LOGINRESULT) };
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

    // Create a socket and request
    auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECKEQUALRET(sock, INVALID_SOCKET);
    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(4567);
    ret = connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    CHECKEQUALRET(ret, SOCKET_ERROR);

    LoginInfo logininfo;;
    strcpy(logininfo.user, "gongluck");
    strcpy(logininfo.password, "password123");
    ret = send(sock, reinterpret_cast<char*>(&logininfo), sizeof(logininfo), 0);
    CHECKNEQUALRET(ret, sizeof(logininfo));
    ProtocolHeader header;
    ret = recv(sock, reinterpret_cast<char*>(&header), sizeof(header), 0);
    CHECKNEQUALRET(ret, sizeof(header));
    std::cout << "header.type is " << header.type
        << ", header.totallen is " << header.totallen << std::endl;
    LoginResult loginresult;
    ret = recv(sock, reinterpret_cast<char*>(&loginresult) + sizeof(ProtocolHeader), sizeof(loginresult) - sizeof(ProtocolHeader), 0);
    CHECKNEQUALRET(ret, sizeof(loginresult) - sizeof(ProtocolHeader));
    std::cout << "loginresult.ret is " << loginresult.ret
        << ", loginresult.msg is " << loginresult.msg << std::endl;

    ret = closesocket(sock);
    CHECKEQUALRET(ret, SOCKET_ERROR);

    // Clean up the environment
    ret = WSACleanup();
    CHECKNEQUALRET(ret, 0);
    return 0;
}