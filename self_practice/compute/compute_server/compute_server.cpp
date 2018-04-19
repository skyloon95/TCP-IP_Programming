// server.cpp
// server

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 10
#define OPSZ 4

void ErrorHandling(char *message);
int calculate(int opnum, int opnds[], char operate[]);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	char opinfo[BUF_SIZE];
	int result, opndCnt, i;
	int recvCnt, recvLen;
	int szClntAddr;	//	size of ClntAddr


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
		opndCnt = 0;
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		recv(hClntSock, (char*)&opndCnt, 1, 0);

		recvLen = 0;
		while (((opndCnt*OPSZ) + opndCnt - 1 ) > recvLen) {
			recvCnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE - 1, 0);
			recvLen += recvCnt;
		}
		

		result = calculate(opndCnt, (int*)opinfo, &opinfo[opndCnt*OPSZ]);
		send(hClntSock, (char*)&result, sizeof(result), 0);
		closesocket(hClntSock);
	}

	closesocket(hServSock);

	WSACleanup();

	return 0;
}

int calculate(int opnum, int opnds[], char operate[]) {
	int i;
	int result;

	for (i = 0; i < opnum; i++) {
		printf("%d ", opnds[i]);
	}

	for (i = 0; i < (opnum - 1); i++) {
		printf("%c ", operate[i]);
	}

	result = opnds[0];
	for (i = 1; i < opnum; i++) {
		switch (operate[i-1])
		{
		case '+':
			result += opnds[i];
			break;
			
		case '-':
			result -= opnds[i];
			break;

		case '*':
			result *= opnds[i];
			break;

		default:
			break;
		}
	}

	return result;
}



void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

