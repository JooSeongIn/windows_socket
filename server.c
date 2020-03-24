#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char *message);

int main()
{
	// WSADATA ����ü�� Windows ���� ������ ���� ������ ��� �ִ�.
	WSADATA wsaData; 
	SOCKET hServSock, hClntSock;
	// SOCKADDR ����ü���� sa_family�� AF_INET �� ��쿡 ����ϴ� ����ü
	// SOCKADDR ����ü�� sa_data�� IP�� Port�� ���յǾ� �־� ���ų� �б� �����ϱ� ������ SOCKADDR_IN ����ü�� �̿��Ѵ�.
	SOCKADDR_IN servAddr, clntAddr;

	int sz_clntAddr;
	char message[] = "test";
	
	// WSAStartup : Initiates use of the Winsock DLL by a process.
	// MAKEWORD(2, 2) : wVersion 2.2
	// if successful, returns 0
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	// socket(int af, int type, int protocol)�� ���� ����
	// PF_INET : IPv4 (AF_INET�� �����ϳ�, ���������� �������� ü�踦 ǥ���� ���� PF��, �ּ� ü�踦 ǥ���� ���� AF�� ���)
	// SOCK_STREAM : TCP
	// If no error occurs, socket returns a descriptor referencing the new socket.
	// Otherwise, a value of INVALID_SOCKET is returned.
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if(hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	// htonl : converts a u_long from host to TCP/IP network byte order.
	// INADDR_ANY : binds the socket to all available interfaces.
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// htons : converts a u_short from host to TCP/IP network byte order.
	servAddr.sin_port = htons(8888);
	
	// bind : associates(����) a local address with a socket.
	if(bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	
	// listen : places a socket in a state in which it is listening for an incoming connection.
	// listen(Socket s, int backlog) : backlog�� accept()���� ��� ������ connection request�� �ִ� ����
	if(listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	
	sz_clntAddr = sizeof(clntAddr);

	// accept : permits an incoming connection attempt on a socket.
	// accept(SOCKET s, sockaddr *addr(SOCKADDR_IN ����ü�� ����ȯ �ʿ�), int *addrlen);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &sz_clntAddr);
	if(hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	// send : sends data on a connected socket.
	send(hClntSock, message, sizeof(message), 0);

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
