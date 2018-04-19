#include <stdio.h>

#define OPSZ 4

int main() {
	int opndCnt;
	char message[1024];
	int i, j;

	fputs("How many numbers do you want to compute? : ", stdout);
	scanf("%d", &opndCnt);
	getchar();
	message[0] = (char)opndCnt;

	j = 0;

	for (i = 0; i < opndCnt; i++) {
		printf("%dth number : ", i + 1);
		scanf("%d", (int*)&message[i*OPSZ + 1 + j]);
		getchar();

		if (i != (opndCnt - 1)) {
			printf("%dth operator : ", i + 1);
			scanf("%c", &message[(i + 1)*OPSZ + 1 + j]);
			getchar();
			j++;
		}
	}

	for (int x = 0; x < (i*OPSZ + 1 + j); x++) {
		printf("%c", message[x]);
	}

	return 0;
}