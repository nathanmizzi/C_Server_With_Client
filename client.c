#include <arpa/inet.h>
#include <unistd.h>
#include "responses.h"

void resetBuffer(char *buffer){
	strcpy(buffer,"");
}

#ifdef Full

	#include <time.h>

	void makeLog(char *buffer, char *recvline, char *fileName){

		time_t t;
		time(&t);

		strncat(buffer,recvline,strlen(recvline) + 1);
		strncat(buffer,ctime(&t),strlen(ctime(&t)) + 1);
		strncat(buffer,"\n",2);

		writeToFile(buffer,fileName,".txt","a");
		resetBuffer(buffer);

	}

#endif

int main(int argc, char *argv[]) {

	#ifdef Full

		char fileName[STR_MAX] = "log";

		if(argc == 2){
			strcpy(fileName,argv[1]);
		}else{
			printf("No arguments supplied, using default logname: log\n");
		}

	#endif

	char buffer[STR_MAX] = "";
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

			#ifdef Full
				makeLog(buffer,recvline,fileName);
			#endif

			int recvInt = atof(recvline);

			if(recvInt < 100)
				printf("User heart rate is low at %d beats per minute\n", recvInt);
			else
				printf("User heart rate is nominal at %d beats per minute\n", recvInt);
			

		}else if(strcmp(sendline, "TEMP\n") == 0){

			#ifdef Full
				makeLog(buffer,recvline,fileName);
			#endif

			int recvFloat = atof(recvline);

			if(recvFloat > 37.8)
				printf("User Has a Fever!\n");
			else if(recvFloat >= 37.0 && recvFloat <= 37.7) 
				printf("User is Normal\n");
			else
				printf("User's temperature is Low!\n");
			

		}else if(strcmp(sendline, "STEPS\n") == 0){

			#ifdef Full
				makeLog(buffer,recvline,fileName);
			#endif

			int recvInt = atof(recvline);

			if(recvInt < 6000 && recvInt > 0)
				printf("User only walked %d steps so far today\n", recvInt);
			else if(recvInt > 6000)
				printf("User has succesfully walked %d steps today!\n", recvInt);
			else
				printf("Steps have been reset to zero by reset command\n");
			

		}else if(strcmp(sendline, "STATS\n") == 0){

			#ifdef Full
				makeLog(buffer,recvline,fileName);
			#endif

			unsigned int battery = 0;
			unsigned int rate = 0;
			unsigned int temp = 0;
			unsigned int steps = 0;
			unsigned int cnt = 0;
			char *token;

			token = strtok(recvline,":");

			while( token != NULL ) {

				cnt++;

				str_getNumbers(token,buffer);

				if(cnt == 2){
					battery = atoi(buffer);
					resetBuffer(buffer);
				}else if(cnt == 3){
					rate = atoi(buffer);
					resetBuffer(buffer);
				}else if(cnt == 4){
					temp = atoi(buffer);
					resetBuffer(buffer);					
				}else if(cnt ==5){
					steps = atoi(buffer);
					resetBuffer(buffer);
				}

				resetBuffer(buffer);
				token = strtok(NULL, ":");
			}

			printf("Battery Level is now %d%% and number of calls recorded for each command since last boot/reset are %d, %d and %d respectively.\n", battery,rate,temp,steps);

		}else if(strcmp(sendline, "RESET\n") == 0){

			#ifdef Full
				makeLog(buffer,recvline,fileName);
			#endif

			printf("All Values Cleared!\n");

		}else{

			#ifdef Full
				makeLog(buffer,recvline,fileName);
			#endif

			printf("%s" ,recvline);
		}
		

	}

	close(sockfd);
	return 0;
}
