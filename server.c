#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "responses.h"

int main(void){
    char str[STR_MAX];
	int listen_fd, comm_fd;
	struct sockaddr_in servaddr;

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

	while (1) { //keep accepting new clients
		comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
		if (comm_fd == -1) {
			fprintf(stderr, "ERROR: accept()\n");
			return -5;
		}

		while (1) { //keep reading from connected client
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

			printf("Echoing back - %s\n",str);

			if (write(comm_fd, str, strlen(str)+1) < 0){
				fprintf(stderr, "ERROR: read()\n");
				return -7;
			}

		}
	}

}