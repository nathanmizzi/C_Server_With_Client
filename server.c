#include <unistd.h>
#include <netdb.h>
#include "responses.h"
#include <ctype.h>

struct recvdCommands {
	unsigned int rateCnt;
	unsigned int tempCnt;
	unsigned int stepsCnt;
};

void resetBuffer(char *buffer){
	strcpy(buffer,"");
}

void resetStruct(struct recvdCommands *structToReset){
	structToReset->rateCnt = 0;
	structToReset->tempCnt = 0;
	structToReset->stepsCnt = 0;
	remove("log.csv");
}

void convertToCsv(struct recvdCommands *structToConvertToCsv, char *stringToSaveCsvIn){
	char csvString[STR_MAX];
	sprintf(csvString, "rateCnt,%d\ntempCnt,%d\nstepsCnt,%d", structToConvertToCsv->rateCnt, structToConvertToCsv->tempCnt, structToConvertToCsv->stepsCnt);
	strcpy(stringToSaveCsvIn,csvString);
}

void readFromFile(struct recvdCommands *structToFill, char *buffer){

	unsigned int cnt = 0;
	char line[STR_MAX];

	if(fopen("records.csv", "r")){

		FILE *fp = fopen("records.csv", "r");

		while(fgets(line,STR_MAX,fp)){

			resetBuffer(buffer);

			if(cnt == 0){
				str_getNumbers(line,buffer);
				structToFill->rateCnt = atoi(buffer);
			}else if(cnt == 1){
				str_getNumbers(line,buffer);
				structToFill->tempCnt = atoi(buffer);
			}else if(cnt == 2){
				str_getNumbers(line,buffer);
				structToFill->stepsCnt = atoi(buffer);
			}

			cnt++;

		}

		fclose(fp);
	}else{
		resetStruct(structToFill);
	}
}

int main(void){
    char str[STR_MAX];
	char buffer[STR_MAX] = "";
	int listen_fd, comm_fd;
	struct sockaddr_in servaddr;
	struct recvdCommands cmdsCounter;
	char recordsFileName[105] = "records";
	int steps = rand() % 10000 + 0;

	readFromFile(&cmdsCounter, buffer);

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_fd == -1) {
		fprintf(stderr, "ERROR: socket()\n");
		return -1;
	}

	int enable_reuse = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int)) < 0) {
		fprintf(stderr, "ERROR: setsockopt(SO_REUSEADDR) failed\n");
		return -2;
	}

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servaddr.sin_port = htons(LISTEN_PORT);

    if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        fprintf(stderr, "ERROR: bind()\n");
        return -3;
	}

	if (listen(listen_fd, 10) == -1) {
		fprintf(stderr, "ERROR: listen()\n");
		return -4;
	}

	while (1) {
		comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
		if (comm_fd == -1) {
			fprintf(stderr, "ERROR: accept()\n");
			return -5;
		}

		while (1) {
			memset(str, 0, STR_MAX);
			int num_bytes = read(comm_fd, str, STR_MAX-1);
			if (num_bytes == -1) {
				fprintf(stderr, "ERROR: read()\n");
				return -6;
			}
			else if (num_bytes == 0) {
				printf("Client disconnected normally, waiting for next client...\n");
				break;
			}

            if(strcmp(str,"RATE\n") == 0){

				cmdsCounter.rateCnt++;

				convertToCsv(&cmdsCounter,buffer);
				writeToFile(buffer,recordsFileName,".csv","w");

				resetBuffer(buffer);

                int random_number = rand() % 250 + 0;

                sprintf(str,"%d\n",random_number);

            }else if(strcmp(str,"TEMP\n") == 0){

				cmdsCounter.tempCnt++;

				convertToCsv(&cmdsCounter,buffer);
				writeToFile(buffer,recordsFileName,".csv","w");

				resetBuffer(buffer);

                sprintf(str,"%0.1f\n", 37.4f);

			}else if(strcmp(str,"STEPS\n") == 0){

				cmdsCounter.stepsCnt++;

				convertToCsv(&cmdsCounter,buffer);
				writeToFile(buffer,recordsFileName,".csv","w");

				resetBuffer(buffer);

                sprintf(str,"%d\n",steps);

			}else if(strcmp(str,"STATS\n") == 0){

				int random_number = rand() % 100 + 1;

                sprintf(str,"BATT:%d,RATE:%d,TEMP:%d,STEPS:%d\n",random_number, cmdsCounter.rateCnt, cmdsCounter.tempCnt, cmdsCounter.stepsCnt);

			}else if(strcmp(str,"RESET\n") == 0){

				resetStruct(&cmdsCounter);

				steps = 0;

                sprintf(str,"OK\n");

			}else{
				sprintf(str,"UNKNOWN\n");
			}

			printf("\nEchoing back - %s\n",str);

			if (write(comm_fd, str, strlen(str)+1) < 0){
				fprintf(stderr, "ERROR: read()\n");
				return -7;
			}

		}
	}

}