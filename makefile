build_all: debug_normal release_normal debug_full release_full

rebuild_all: clean_all debug_normal release_normal debug_full release_full

debug_normal:
	gcc -Wall -DNormal -o  bin/dbg/clientNormal client.c 
	gcc -Wall -DNormal -o  bin/dbg/serverNormal server.c 

release_normal: 
	gcc -Wall -DNormal -o bin/rel/clientNormal client.c
	gcc -Wall -DNormal -o bin/rel/serverNormal server.c

debug_full:
	gcc -Wall -DFull -o  bin/dbg/clientFull client.c 
	gcc -Wall -DFull -o  bin/dbg/serverFull server.c 

release_full: 
	gcc -Wall -DFull -o bin/rel/clientFull client.c
	gcc -Wall -DFull -o bin/rel/serverFull server.c

clean_all:
	-rm bin/dbg/clientNormal bin/dbg/serverNormal bin/rel/clientNormal bin/rel/serverNormal bin/dbg/clientFull bin/dbg/serverFull bin/rel/clientFull bin/rel/serverFull
