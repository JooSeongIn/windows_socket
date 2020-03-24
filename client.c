#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define MSG_LEN	30

void ErrorHandling(char *msg);

int main(void)
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[MSG_LEN];
	int strLen;
	
	// 家南 扼捞宏矾府 檬扁拳
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	// 家南 积己
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(8888);

	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if(strLen == -1)
		ErrorHandling("recv() error");

	printf("Message from server : %s\n", message);
	closesocket(hSocket);
	WSACleanup();
	
	system("pause");

	return 0;
}

void ErrorHandling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
