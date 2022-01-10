build_all: debug release

rebuild_all: clean_all debug release

debug:
	gcc -Wall -g -O0 -DNormal -o  bin/dbg/client client.c 
	gcc -Wall -g -O0 -o  bin/dbg/server server.c 

release: 
	gcc -Wall -DFull -o bin/rel/client client.c
	gcc -Wall -o bin/rel/server server.c

clean_all:
	-rm bin/dbg/server bin/dbg/client bin/rel/server bin/dbg/client
