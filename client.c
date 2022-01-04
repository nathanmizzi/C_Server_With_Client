#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "responses.h"

int main(void) {
	char sendline[STR_MAX], recvline[STR_MAX];
	struct sockaddr_in servaddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1) {
		fprintf(stderr, "socket() failed\n");
		return 1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servaddr.sin_port = htons(LISTEN_PORT);

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		fprintf(stderr, "connect() failed\n");
		return 2;
	}

	while (1) {
		memset(sendline, 0, STR_MAX);
		memset(recvline, 0, STR_MAX);
		printf("Enter your request: ");
		fgets(sendline, STR_MAX-1, stdin);
		if (write(sockfd, sendline, strlen(sendline)+1) < 0) {
			fprintf(stderr, "write() failed\n");
			return 3;
		}

		if (read(sockfd, recvline, STR_MAX-1) < 0) {
			fprintf(stderr, "read() failed\n");
			return 4;
		}
		
		printf("Received: %s", recvline);
	}

	close(sockfd);
	return 0;
}
