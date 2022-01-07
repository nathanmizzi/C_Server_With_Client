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

void str_getNumbers(char *stringToSearch, char *stringToSaveTo){

    char cntValue[STR_MAX];

    for(int i = 0; i < strlen(stringToSearch); i++){
        if(isdigit(stringToSearch[i]) > 0){
            strncat(cntValue,&stringToSearch[i],1);
        }	
    }

    strcpy(stringToSaveTo,cntValue);
    strcpy(cntValue,"");
}


#endif
