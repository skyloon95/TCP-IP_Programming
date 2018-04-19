// client.cpp
// client

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[BUF_SIZE];
	int result, opndCnt, i, j;


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
	else {
		puts("Connected.........\n");
	}

	fputs("How many numbers do you want to compute? : ", stdout);
	scanf("%d", &opndCnt);
	getchar();
	message[0] = (char)opndCnt;

	j = 1;

	for (i = 0; i < opndCnt; i++) {
		printf("%dth number : ", i + 1);
		scanf("%d", (int*)&message[i*OPSZ+1]);
		getchar();

		if (i != (opndCnt - 1)) {
			printf("%dth operator : ", i + 1);
			scanf("%c", &message[(opndCnt*OPSZ) + j]);
			getchar();
			j++;
		}
	}

	send(hSocket, message, opndCnt*OPSZ  + j, 0);
	recv(hSocket, (char*)&result, RLT_SIZE, 0);

	printf("Compute result : %d \n", result);

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

