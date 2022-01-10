#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
    for(int i = 0; i < strlen(stringToSearch); i++){
        if(isdigit(stringToSearch[i]) > 0){
            strncat(stringToSaveTo,&stringToSearch[i],1);
        }	
    }
}

void writeToFile(char *textToWrite, char *nameOfFile, char fileType[], char *writeMode){

    char tempNameOfFile[100];

    strcpy(tempNameOfFile,nameOfFile);

    strncat(tempNameOfFile, fileType, 5);

    FILE *fp = fopen(tempNameOfFile, writeMode);
    fprintf(fp,"%s", textToWrite);
    fclose(fp);

}


#endif
