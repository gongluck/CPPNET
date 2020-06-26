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
		printf("������˿�ʧ��...\n");
	}
	else
	{
		printf("������˿ڳɹ�...\n");
	}
	if (listen(_sock, 5) == SOCKET_ERROR)
	{
		printf("��������˿�ʧ��...\n");
	}
	else
	{
		printf("��������˿ڳɹ�...\n");
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
			printf("���յ���Ч�ͻ���SOCKET...\n");
		}
		printf("�¿ͻ������ӣ�IP=%s\n", inet_ntoa(clientAddr.sin_addr));
		send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	}

	closesocket(_sock);

	WSACleanup();
	return 0;
}