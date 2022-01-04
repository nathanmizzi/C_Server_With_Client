all: clean server client

tcp_server: 
	gcc -Wall -o server server.c

client: 
	gcc -Wall -o client client.c

clean:
	-rm server client