build_all: debug release

rebuild_all: clean_all debug release

debug:
	gcc -Wall -o bin/dbg/client client.c 
	gcc -Wall -o bin/dbg/server server.c 

release: 
	gcc -Wall -o bin/rel/client client.c
	gcc -Wall -o bin/rel/server server.c

clean_all:
	-rm bin/rel/client bin/rel/server bin/dbg/client bin/dbg/server
