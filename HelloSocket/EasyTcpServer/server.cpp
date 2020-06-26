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

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR)
	{
		printf("绑定网络端口失败...\n");
	}
	else
	{
		printf("绑定网络端口成功...\n");
	}
	if (listen(_sock, 5) == SOCKET_ERROR)
	{
		printf("监听网络端口失败...\n");
	}
	else
	{
		printf("监听网络端口成功...\n");
	}
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(clientAddr);
	SOCKET _cSock = INVALID_SOCKET;
	char msgBuf[] = "Hello, I'm Server.";
	
	while (true)
	{
		_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
		if (_cSock == INVALID_SOCKET)
		{
			printf("接收到无效客户端SOCKET...\n");
		}
		printf("新客户端连接：IP=%s\n", inet_ntoa(clientAddr.sin_addr));
		send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	}

	closesocket(_sock);

	WSACleanup();
	return 0;
}