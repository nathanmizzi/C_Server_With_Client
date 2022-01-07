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
		printf("\nEnter A Request [ex: TEMP,STEPS] : ");
		fgets(sendline, STR_MAX-1, stdin);

		str_toupper(sendline);

		if (write(sockfd, sendline, strlen(sendline)+1) < 0) {
			fprintf(stderr, "write() failed\n");
			return 3;
		}

		if (read(sockfd, recvline, STR_MAX-1) < 0) {
			fprintf(stderr, "read() failed\n");
			return 4;
		}
		
		if(strcmp(sendline, "RATE\n") == 0){

			int recvInt = atof(recvline);

			if(recvInt < 100)
				printf("Low Rate! : %s", recvline);
			else
				printf("Good Rate! : %s", recvline);
			

		}else if(strcmp(sendline, "TEMP\n") == 0){

			int recvFloat = atof(recvline);

			if(recvFloat > 37.8)
				printf("Fever! : %s", recvline);
			else if(recvFloat >= 37.0 && recvFloat <= 37.7) 
				printf("Normal! : %s", recvline);
			else
				printf("Low! : %s", recvline);
			

		}else if(strcmp(sendline, "STEPS\n") == 0){

			int recvInt = atof(recvline);

			if(recvInt < 6000)
				printf("Step Goal Not Reached! : %s", recvline);
			else
				printf("Good Job, Step Goal Reached! : %s", recvline);
			

		}else{
			printf("%s" ,recvline);
		}
		

	}

	close(sockfd);
	return 0;
}
