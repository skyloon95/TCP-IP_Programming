// server.cpp
// server

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 10
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int result, recvNum;
	int szClntAddr;	//	size of ClntAddr
	int count;
	int randNum;
	int recvLen, recvCnt;
	char message[20];
	int* tmp;


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

	count = 0;
	randNum = rand() % 101;

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	printf("연결 성공...\n");
	printf("난수 생성 : ???\n");
	printf("num : %d\n", randNum);

	while (1) {
		result = -1;
		recvNum = -1;
		
		recvLen = 0;
		while ((OPSZ) > recvLen) {
			recvCnt = recv(hClntSock, &message[recvLen], 20 - 1, 0);
			recvLen += recvCnt;
		}

		printf("recv num : %d\n", recvNum);

		if (recvNum == randNum) {
			result = 1;
			break;
		}
		else if (recvNum > randNum) {
			result = 2;
			count++;
		}
		else if (recvNum < randNum) {
			result = 3;
			count++;
		}
	}

	result = count + 10000;

	send(hClntSock, (char*)&result, sizeof(result), 0);
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

