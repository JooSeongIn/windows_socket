#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char *message);

int main()
{
	// WSADATA 구조체는 Windows 소켓 구현을 위한 정보를 담고 있다.
	WSADATA wsaData; 
	SOCKET hServSock, hClntSock;
	// SOCKADDR 구조체에서 sa_family가 AF_INET 인 경우에 사용하는 구조체
	// SOCKADDR 구조체는 sa_data에 IP와 Port가 조합되어 있어 쓰거나 읽기 불편하기 때문에 SOCKADDR_IN 구조체를 이용한다.
	SOCKADDR_IN servAddr, clntAddr;

	int sz_clntAddr;
	char message[] = "test";
	
	// WSAStartup : Initiates use of the Winsock DLL by a process.
	// MAKEWORD(2, 2) : wVersion 2.2
	// if successful, returns 0
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	// socket(int af, int type, int protocol)로 소켓 생성
	// PF_INET : IPv4 (AF_INET과 동일하나, 형식적으로 프로토콜 체계를 표현할 때는 PF를, 주소 체계를 표현할 때는 AF를 사용)
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
	
	// bind : associates(결합) a local address with a socket.
	if(bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	
	// listen : places a socket in a state in which it is listening for an incoming connection.
	// listen(Socket s, int backlog) : backlog는 accept()없이 대기 가능한 connection request의 최대 개수
	if(listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	
	sz_clntAddr = sizeof(clntAddr);

	// accept : permits an incoming connection attempt on a socket.
	// accept(SOCKET s, sockaddr *addr(SOCKADDR_IN 구조체의 형변환 필요), int *addrlen);
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
