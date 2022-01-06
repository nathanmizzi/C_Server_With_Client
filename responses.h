#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>

#define STR_MAX	1024
#define LISTEN_PORT	54321

void str_toupper(char *string) {
	unsigned short i = 0;
	 while (string[i] != 0) { 
        string[i] = toupper(string[i]); 
        i++; 
    } 
}


#endif
