// server.cpp
// server

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;	//	size of ClntAddr
	char message[BUF_SIZE];
	int strLen, i;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	//	reset WinSock Library for current version. if not matches version, call ErrorHandling
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	//	Create ServSock. If it is invalid socket, call ErrorHandling.
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//	Bindling. If it fails, call ErrorHandling
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

	//	Listening. If it fails, call ErrorHandling
	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	szClntAddr = sizeof(clntAddr);

	for (i = 0; i < 5; i++) {
		//	Save return of accept(pram with hServSock) at hClntSock.
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET) {
			ErrorHandling("accept() error");
		}
		else {
			printf("Connected client %d \n", i + 1);
		}
		
		while((strLen=recv(hClntSock, message, BUF_SIZE, 0))!=0)
			send(hClntSock, message, strLen, 0);
		closesocket(hClntSock);
	}
	
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

