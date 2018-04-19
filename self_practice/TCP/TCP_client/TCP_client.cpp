// Hello_client.cpp
// client

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int idx = 0, readLen = 0;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	//	reset WinSock Library for current version. if not matches version, call ErrorHandling
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	//	Create Socket. If it is invalid socket, call ErrorHandling.
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));	//	reset addr to 0
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	//	Connet to server. If it fais, call ErrorHandling
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	while (readLen = recv(hSocket, &message[idx++], 1, 0)) {
		if (readLen == -1) {
			ErrorHandling("read() error!");
		}

		strLen += readLen;
	}

	//	print message from server
	printf("Message from server : %s \n", message);
	printf("Function read call count : %d \n", strLen);

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

