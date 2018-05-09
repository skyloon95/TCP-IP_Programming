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

	int result, inputNum;
	char message[20];


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

	while (1) {
		result = -1;
		fputs("숫자를 맞추시오 : ", stdout);
		scanf("%d", (int*)&message[0]);
		getchar();

		send(hSocket, message, OPSZ, 0);
		printf("num sended : \n");
		recv(hSocket, (char*)&result, RLT_SIZE, 0);

		if (result == 2) {
			printf("서버의 수는 입력하신 수 보다 작습니다\n\n");
		}
		else if (result == 3) {
			printf("서버의 수는 입력하신 수 보다 큽니다\n\n");
		}
		else {
			printf("정답을 %d번만에 맞추셨습니다. 클라이언트를 종료합니다", result - 10000);
			break;
		}
	}

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

