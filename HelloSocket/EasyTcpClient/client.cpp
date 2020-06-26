//#define WIN32_LEAN_AND_MEAN    

#include <Windows.h>
#include <WinSock2.h>

#include <stdio.h>

//#pragma comment(lib, "ws2_32.lib")

int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET)
	{
		printf("创建套接字失败...\n");
	}
	else
	{
		printf("创建套接字成功...\n");
	}
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (ret == SOCKET_ERROR)
	{
		printf("连接失败...\n");
	}
	else
	{
		printf("连接成功...\n");
	}
	char recvBuf[256] = {};
	int nLen = recv(_sock, recvBuf, 256, 0);
	if (nLen > 0)
	{
		printf("接收到数据：%s\n", recvBuf);
	}

	WSACleanup();
	getchar();
	return 0;
}