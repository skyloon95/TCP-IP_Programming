// inet_adrconv_win.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string.h>
#include <winsock2.h>

void ErrorHandling(char *message);


int main(int argc, char *argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/*inet_addr 함수 호출 예*/
	{
		char *addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr);
		if (conv_addr == INADDR_NONE)
			printf("Error occured!\n");
		else
			printf("Network ordered integer addr: %#lx \n", conv_addr);

	}

	/*inet_ntoa 함수 호출 예*/
	{
		struct sockaddr_in addr;
		char *strPtr;
		char strArr[20];

		addr.sin_addr.s_addr = htonl(0x10203040);
		strPtr = inet_ntoa(addr.sin_addr);
		strcpy(strArr, strPtr);
		printf("Dotted-Decimal notation3 %s \n", strArr);
	}

	WSACleanup();

    return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

